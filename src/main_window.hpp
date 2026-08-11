#ifndef main_window_hpp
#define main_window_hpp

#include "radio_station.hpp"

#include <QMainWindow>

class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QSpinBox;
class QTableWidget;

class main_window final : public QMainWindow
{
private:
    radio_station station_;

    QLabel* now_playing_title_ = nullptr;
    QLabel* now_playing_artist_ = nullptr;
    QLabel* station_summary_ = nullptr;
    QLabel* audience_summary_ = nullptr;
    QListWidget* playlist_list_ = nullptr;
    QTableWidget* programs_table_ = nullptr;
    QLineEdit* title_input_ = nullptr;
    QLineEdit* artist_input_ = nullptr;
    QSpinBox* duration_input_ = nullptr;
    QPushButton* broadcast_button_ = nullptr;
    QPushButton* remove_button_ = nullptr;

    void build_interface();
    void apply_style();
    void refresh_interface();
    void refresh_playlist();
    void refresh_programs();
    void refresh_summary();
    void update_action_state();

    void add_music();
    void remove_selected_music();
    void broadcast_selected_music();
    void save_station();
    void load_station();

public:
    explicit main_window(QWidget* parent = nullptr);
};

#endif
