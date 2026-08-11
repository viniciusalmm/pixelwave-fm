#include "main_window.hpp"

#include "demo_data.hpp"
#include "errors.hpp"
#include "music.hpp"
#include "repository.hpp"
#include "service.hpp"
#include "utils.hpp"

#include <QAbstractItemView>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QWidget>

#include <exception>
#include <memory>
#include <string>

namespace
{
QString format_duration(int duration_seconds)
{
    const int minutes = duration_seconds / 60;
    const int seconds = duration_seconds % 60;
    return QString{"%1:%2"}
        .arg(minutes)
        .arg(seconds, 2, 10, QLatin1Char{'0'});
}
}

main_window::main_window(QWidget* parent)
    : QMainWindow{parent},
      station_{create_demo_station()}
{
    build_interface();
    apply_style();
    refresh_interface();
}

void main_window::build_interface()
{
    setWindowTitle("PixelWave FM");
    resize(1040, 700);
    setMinimumSize(860, 620);

    auto* central_widget = new QWidget{this};
    auto* root_layout = new QVBoxLayout{central_widget};
    root_layout->setContentsMargins(24, 20, 24, 20);
    root_layout->setSpacing(16);

    auto* header_layout = new QHBoxLayout{};
    auto* title_block = new QVBoxLayout{};

    auto* app_title = new QLabel{"PixelWave FM", central_widget};
    app_title->setObjectName("app_title");
    auto* app_subtitle = new QLabel{
        "Painel de demonstração da estação de rádio", central_widget};
    app_subtitle->setObjectName("app_subtitle");

    title_block->addWidget(app_title);
    title_block->addWidget(app_subtitle);

    auto* on_air_badge = new QLabel{"●  NO AR", central_widget};
    on_air_badge->setObjectName("on_air_badge");
    on_air_badge->setAlignment(Qt::AlignCenter);
    on_air_badge->setFixedWidth(110);

    header_layout->addLayout(title_block);
    header_layout->addStretch();
    header_layout->addWidget(on_air_badge);
    root_layout->addLayout(header_layout);

    auto* summary_frame = new QFrame{central_widget};
    summary_frame->setObjectName("summary_frame");
    auto* summary_layout = new QHBoxLayout{summary_frame};
    summary_layout->setContentsMargins(18, 14, 18, 14);

    station_summary_ = new QLabel{summary_frame};
    station_summary_->setObjectName("summary_text");
    audience_summary_ = new QLabel{summary_frame};
    audience_summary_->setObjectName("summary_text");

    summary_layout->addWidget(station_summary_);
    summary_layout->addStretch();
    summary_layout->addWidget(audience_summary_);
    root_layout->addWidget(summary_frame);

    auto* content_layout = new QGridLayout{};
    content_layout->setHorizontalSpacing(16);
    content_layout->setVerticalSpacing(16);
    content_layout->setColumnStretch(0, 3);
    content_layout->setColumnStretch(1, 2);

    auto* playlist_group = new QGroupBox{"Playlist", central_widget};
    auto* playlist_layout = new QVBoxLayout{playlist_group};

    playlist_list_ = new QListWidget{playlist_group};
    playlist_list_->setSelectionMode(QAbstractItemView::SingleSelection);
    playlist_list_->setAlternatingRowColors(true);
    playlist_layout->addWidget(playlist_list_);

    auto* playlist_actions = new QHBoxLayout{};
    broadcast_button_ = new QPushButton{"Transmitir selecionada", playlist_group};
    broadcast_button_->setObjectName("primary_button");
    remove_button_ = new QPushButton{"Remover", playlist_group};
    remove_button_->setObjectName("secondary_button");

    playlist_actions->addWidget(broadcast_button_);
    playlist_actions->addWidget(remove_button_);
    playlist_layout->addLayout(playlist_actions);

    content_layout->addWidget(playlist_group, 0, 0, 2, 1);

    auto* now_playing_group = new QGroupBox{"Agora no ar", central_widget};
    auto* now_playing_layout = new QVBoxLayout{now_playing_group};

    now_playing_title_ = new QLabel{"Nenhuma música selecionada", now_playing_group};
    now_playing_title_->setObjectName("now_playing_title");
    now_playing_title_->setWordWrap(true);

    now_playing_artist_ = new QLabel{
        "Selecione uma faixa da playlist e inicie a transmissão.", now_playing_group};
    now_playing_artist_->setObjectName("muted_text");
    now_playing_artist_->setWordWrap(true);

    now_playing_layout->addWidget(now_playing_title_);
    now_playing_layout->addWidget(now_playing_artist_);
    now_playing_layout->addStretch();
    content_layout->addWidget(now_playing_group, 0, 1);

    auto* add_group = new QGroupBox{"Adicionar música", central_widget};
    auto* add_layout = new QVBoxLayout{add_group};
    auto* form_layout = new QFormLayout{};

    title_input_ = new QLineEdit{add_group};
    title_input_->setPlaceholderText("Ex.: Midnight Signal");
    artist_input_ = new QLineEdit{add_group};
    artist_input_->setPlaceholderText("Ex.: PixelWave Sessions");
    duration_input_ = new QSpinBox{add_group};
    duration_input_->setRange(1, 3600);
    duration_input_->setValue(180);
    duration_input_->setSuffix(" s");

    form_layout->addRow("Título", title_input_);
    form_layout->addRow("Artista", artist_input_);
    form_layout->addRow("Duração", duration_input_);
    add_layout->addLayout(form_layout);

    auto* add_button = new QPushButton{"Adicionar à playlist", add_group};
    add_button->setObjectName("secondary_button");
    add_layout->addWidget(add_button);
    content_layout->addWidget(add_group, 1, 1);

    auto* programs_group = new QGroupBox{"Programação", central_widget};
    auto* programs_layout = new QVBoxLayout{programs_group};

    programs_table_ = new QTableWidget{programs_group};
    programs_table_->setColumnCount(4);
    programs_table_->setHorizontalHeaderLabels(
        {"Programa", "Horário", "Tipo", "Audiência"});
    programs_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    programs_table_->setSelectionMode(QAbstractItemView::NoSelection);
    programs_table_->setAlternatingRowColors(true);
    programs_table_->verticalHeader()->setVisible(false);
    programs_table_->horizontalHeader()->setStretchLastSection(false);
    programs_table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    programs_table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    programs_table_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    programs_table_->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    programs_layout->addWidget(programs_table_);

    content_layout->addWidget(programs_group, 2, 0, 1, 2);
    root_layout->addLayout(content_layout, 1);

    auto* footer_layout = new QHBoxLayout{};
    auto* persistence_hint = new QLabel{
        "Persistência JSON pela mesma camada de serviço usada fora da GUI.", central_widget};
    persistence_hint->setObjectName("muted_text");

    auto* load_button = new QPushButton{"Carregar JSON", central_widget};
    load_button->setObjectName("secondary_button");
    auto* save_button = new QPushButton{"Salvar JSON", central_widget};
    save_button->setObjectName("primary_button");

    footer_layout->addWidget(persistence_hint);
    footer_layout->addStretch();
    footer_layout->addWidget(load_button);
    footer_layout->addWidget(save_button);
    root_layout->addLayout(footer_layout);

    setCentralWidget(central_widget);

    connect(playlist_list_, &QListWidget::currentRowChanged,
            this, [this](int) { update_action_state(); });
    connect(playlist_list_, &QListWidget::itemDoubleClicked,
            this, [this](QListWidgetItem*) { broadcast_selected_music(); });
    connect(broadcast_button_, &QPushButton::clicked,
            this, [this]() { broadcast_selected_music(); });
    connect(remove_button_, &QPushButton::clicked,
            this, [this]() { remove_selected_music(); });
    connect(add_button, &QPushButton::clicked,
            this, [this]() { add_music(); });
    connect(save_button, &QPushButton::clicked,
            this, [this]() { save_station(); });
    connect(load_button, &QPushButton::clicked,
            this, [this]() { load_station(); });
}

void main_window::apply_style()
{
    setStyleSheet(R"(
        QMainWindow, QWidget {
            background: #f4f7fb;
            color: #172033;
            font-family: "Segoe UI", Arial, sans-serif;
            font-size: 14px;
        }

        QLabel#app_title {
            font-size: 28px;
            font-weight: 700;
            color: #13233f;
        }

        QLabel#app_subtitle,
        QLabel#muted_text {
            color: #667085;
        }

        QLabel#on_air_badge {
            color: #ffffff;
            background: #d92d20;
            border-radius: 14px;
            padding: 7px 10px;
            font-weight: 700;
        }

        QFrame#summary_frame,
        QGroupBox {
            background: #ffffff;
            border: 1px solid #dbe3ee;
            border-radius: 10px;
        }

        QLabel#summary_text {
            font-weight: 600;
            color: #344054;
        }

        QGroupBox {
            margin-top: 12px;
            padding-top: 12px;
            font-weight: 700;
            color: #263652;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 6px;
        }

        QListWidget, QTableWidget, QLineEdit, QSpinBox {
            background: #ffffff;
            border: 1px solid #cfd8e6;
            border-radius: 7px;
            padding: 6px;
            selection-background-color: #dbeafe;
            selection-color: #172033;
        }

        QHeaderView::section {
            background: #eef3f9;
            color: #344054;
            border: none;
            border-bottom: 1px solid #dbe3ee;
            padding: 7px;
            font-weight: 700;
        }

        QLabel#now_playing_title {
            font-size: 18px;
            font-weight: 700;
            color: #142b4f;
        }

        QPushButton {
            border-radius: 7px;
            padding: 9px 14px;
            font-weight: 600;
        }

        QPushButton#primary_button {
            background: #175cd3;
            color: #ffffff;
            border: 1px solid #175cd3;
        }

        QPushButton#primary_button:hover {
            background: #1849a9;
        }

        QPushButton#secondary_button {
            background: #ffffff;
            color: #344054;
            border: 1px solid #cfd8e6;
        }

        QPushButton#secondary_button:hover {
            background: #f2f4f7;
        }

        QPushButton:disabled {
            color: #98a2b3;
            background: #eaecf0;
            border-color: #eaecf0;
        }
    )");
}

void main_window::refresh_interface()
{
    refresh_playlist();
    refresh_programs();
    refresh_summary();
    update_action_state();
}

void main_window::refresh_playlist()
{
    const int previous_row = playlist_list_->currentRow();
    playlist_list_->clear();

    const auto& musics = station_.get_playlist().get_musics();
    for (const auto& music_item : musics)
    {
        const QString text = QString{"%1  —  %2    %3"}
            .arg(QString::fromStdString(music_item.get_title()))
            .arg(QString::fromStdString(music_item.get_artist()))
            .arg(format_duration(music_item.get_duration()));
        playlist_list_->addItem(text);
    }

    if (!musics.empty())
    {
        const int last_row = static_cast<int>(musics.size()) - 1;
        playlist_list_->setCurrentRow(previous_row >= 0 && previous_row <= last_row
            ? previous_row
            : 0);
    }
}

void main_window::refresh_programs()
{
    const auto& programs = station_.get_programs();
    programs_table_->setRowCount(static_cast<int>(programs.size()));

    for (int row = 0; row < static_cast<int>(programs.size()); ++row)
    {
        const auto& program = programs[static_cast<std::size_t>(row)];
        programs_table_->setItem(row, 0, new QTableWidgetItem{
            QString::fromStdString(program->get_title())});
        programs_table_->setItem(row, 1, new QTableWidgetItem{
            QString::fromStdString(program->get_schedule())});
        programs_table_->setItem(row, 2, new QTableWidgetItem{
            QString::fromStdString(program->type())});
        programs_table_->setItem(row, 3, new QTableWidgetItem{
            QString::number(program->calculate_audience())});
    }
}

void main_window::refresh_summary()
{
    station_summary_->setText(QString{"%1 músicas  •  %2 ouvintes  •  %3 programas"}
        .arg(static_cast<qulonglong>(station_.get_playlist().size()))
        .arg(static_cast<qulonglong>(station_.get_listeners().size()))
        .arg(static_cast<qulonglong>(station_.get_programs().size())));

    const int total_audience = parallel_audience(station_.get_programs());
    audience_summary_->setText(QString{"Audiência estimada: %1"}.arg(total_audience));
}

void main_window::update_action_state()
{
    const bool has_selection = playlist_list_->currentRow() >= 0;
    broadcast_button_->setEnabled(has_selection);
    remove_button_->setEnabled(has_selection);
}

void main_window::add_music()
{
    try
    {
        const std::string title = title_input_->text().trimmed().toStdString();
        const std::string artist = artist_input_->text().trimmed().toStdString();

        station_.get_playlist().add_music(
            music{title, artist, duration_input_->value()});

        title_input_->clear();
        artist_input_->clear();
        duration_input_->setValue(180);
        refresh_playlist();
        refresh_summary();
        playlist_list_->setCurrentRow(
            static_cast<int>(station_.get_playlist().size()) - 1);
    }
    catch (const domain_error& error)
    {
        QMessageBox::warning(
            this, "Não foi possível adicionar", error.what());
    }
}

void main_window::remove_selected_music()
{
    const int row = playlist_list_->currentRow();
    if (row < 0)
    {
        return;
    }

    const auto& selected_music = station_.get_playlist().get_music(
        static_cast<std::size_t>(row));

    const auto answer = QMessageBox::question(
        this,
        "Remover música",
        QString{"Remover \"%1\" da playlist?"}
            .arg(QString::fromStdString(selected_music.get_title())));

    if (answer != QMessageBox::Yes)
    {
        return;
    }

    station_.get_playlist().remove_music(static_cast<std::size_t>(row));
    refresh_playlist();
    refresh_summary();
}

void main_window::broadcast_selected_music()
{
    const int row = playlist_list_->currentRow();
    if (row < 0)
    {
        return;
    }

    try
    {
        const auto& selected_music = station_.get_playlist().get_music(
            static_cast<std::size_t>(row));

        now_playing_title_->setText(
            QString::fromStdString(selected_music.get_title()));
        now_playing_artist_->setText(
            QString{"%1  •  %2  •  transmissão demonstrativa"}
                .arg(QString::fromStdString(selected_music.get_artist()))
                .arg(format_duration(selected_music.get_duration())));
    }
    catch (const std::exception& error)
    {
        QMessageBox::critical(this, "Erro de transmissão", error.what());
    }
}

void main_window::save_station()
{
    const QString path = QFileDialog::getSaveFileName(
        this,
        "Salvar estado da rádio",
        "pixelwave_state.json",
        "Arquivos JSON (*.json)");

    if (path.isEmpty())
    {
        return;
    }

    try
    {
        auto repository_instance = make_json_repository(path.toStdString());
        station_service service{*repository_instance};
        service.save(station_);
        QMessageBox::information(
            this, "Estado salvo", "O estado da rádio foi salvo em JSON.");
    }
    catch (const std::exception& error)
    {
        QMessageBox::critical(this, "Falha ao salvar", error.what());
    }
}

void main_window::load_station()
{
    const QString path = QFileDialog::getOpenFileName(
        this,
        "Carregar estado da rádio",
        QString{},
        "Arquivos JSON (*.json)");

    if (path.isEmpty())
    {
        return;
    }

    try
    {
        auto repository_instance = make_json_repository(path.toStdString());
        station_service service{*repository_instance};
        station_ = service.load();
        now_playing_title_->setText("Nenhuma música selecionada");
        now_playing_artist_->setText(
            "Selecione uma faixa da playlist e inicie a transmissão.");
        refresh_interface();
        QMessageBox::information(
            this, "Estado carregado", "O estado da rádio foi restaurado do JSON.");
    }
    catch (const std::exception& error)
    {
        QMessageBox::critical(this, "Falha ao carregar", error.what());
    }
}
