#include "errors.hpp"
#include "generic.hpp"
#include "listener.hpp"
#include "music.hpp"
#include "music_program.hpp"
#include "news_program.hpp"
#include "radio_station.hpp"
#include "repository.hpp"
#include "service.hpp"
#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <ranges>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <variant>
#include <vector>

int main()
{
    try
    {
        radio_station station("PixelWave FM");

        station.get_playlist().add_music(music("Snow Storm", "Habbo", 180));
        station.get_playlist().add_music(music("Club Mammoth", "Habbo", 205));
        station.get_playlist().add_music(music("Battle Ball", "Habbo", 194));

        station.add_listener(listener("Vinicius", "Brazil", "Retro"));
        station.add_listener(listener("Carlos", "Portugal", "Dance"));

        station.add_program(std::make_unique<music_program>(
            "Morning Hits", "08:00", 12));
        station.add_program(std::make_unique<news_program>(
            "Pixel News", "12:00", 6));

        // Q1(A): o mesmo template é instanciado com dois tipos diferentes.
        registry<music_program> music_registry;
        registry<news_program> news_registry;

        music_registry.add(music_program("Afternoon Mix", "15:00", 15));
        news_registry.add(news_program("Night News", "21:00", 10));

        const auto registered_music = music_registry.find_first(
            [](const music_program& program)
            {
                return program.get_title() == "Afternoon Mix";
            });

        if (registered_music)
        {
            std::cout << "Registered program: "
                      << registered_music->get().get_title() << '\n';
        }

        std::vector<music_program> music_programs{
            music_program("Morning Hits", "08:00", 12),
            music_program("Afternoon Mix", "15:00", 15)
        };

        std::vector<news_program> news_programs{
            news_program("Pixel News", "12:00", 6),
            news_program("Night News", "21:00", 10)
        };

        std::cout << "\n===== GENERIC + CRTP =====\n";
        std::cout << "Music programs audience: "
                  << total_audience(music_programs) << '\n';
        std::cout << "News programs audience: "
                  << total_audience(news_programs) << '\n';
        std::cout << "music_program alive: "
                  << counted<music_program>::alive() << '\n';
        std::cout << "news_program alive: "
                  << counted<news_program>::alive() << '\n';

        // Q1(E): pipeline com dois adaptadores encadeados: filter + transform.
        std::cout << "\n===== RANGES =====\n";
        auto names = station.get_programs()
            | std::views::filter([](const auto& program)
            {
                return program->calculate_audience() >= 1500;
            })
            | std::views::transform([](const auto& program)
            {
                return program->get_title();
            });

        for (const auto& name : names)
        {
            std::cout << name << '\n';
        }

        // Q2(B): optional demonstra busca com sucesso e falha.
        std::cout << "\n===== OPTIONAL =====\n";
        const auto found = station.find_listener("Vinicius");
        const auto missing = station.find_listener("Nobody");

        if (found)
        {
            std::cout << "Found: " << found->get_nickname() << '\n';
        }

        if (!missing)
        {
            std::cout << "Nobody was found.\n";
        }

        // Q2(C): variant representa music ou mensagem de erro e trata as duas alternativas.
        std::cout << "\n===== VARIANT =====\n";
        const auto show_search_result = [](const music_search_result& search_result)
        {
            std::visit([](const auto& value)
            {
                using value_type = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<value_type, music>)
                {
                    std::cout << "Found music: " << value.get_title() << '\n';
                }
                else
                {
                    std::cout << value << '\n';
                }
            }, search_result);
        };

        show_search_result(find_music(
            station.get_playlist().get_musics(), "Snow Storm"));
        show_search_result(find_music(
            station.get_playlist().get_musics(), "Missing Song"));

        // Q2(D): exceção específica capturada pela exceção base do domínio.
        std::cout << "\n===== EXCEPTIONS =====\n";
        try
        {
            station.get_playlist().add_music(music("", "Invalid", 0));
        }
        catch (const domain_error& error)
        {
            std::cout << error.what() << '\n';
        }

        // Q3(A): dois contêineres STL diferentes e adequados ao domínio.
        std::map<std::string, int> music_duration_by_title;
        std::unordered_set<std::string> listener_countries;

        for (const auto& music_item : station.get_playlist().get_musics())
        {
            music_duration_by_title[music_item.get_title()] = music_item.get_duration();
        }

        for (const auto& listener_item : station.get_listeners())
        {
            listener_countries.insert(listener_item.get_country());
        }

        std::cout << "\n===== STL CONTAINERS =====\n";
        std::cout << "Indexed musics: " << music_duration_by_title.size() << '\n';
        std::cout << "Unique listener countries: " << listener_countries.size() << '\n';

        // Q3(B): sort, count_if e accumulate, incluindo lambda com captura.
        auto musics = station.get_playlist().get_musics();
        std::sort(
            musics.begin(),
            musics.end(),
            [](const music& left, const music& right)
            {
                return left.get_duration() < right.get_duration();
            });

        const int duration_limit = 190;
        const auto long_musics = std::count_if(
            musics.begin(),
            musics.end(),
            [duration_limit](const music& music_item)
            {
                return music_item.get_duration() > duration_limit;
            });

        const int total_duration = std::accumulate(
            musics.begin(),
            musics.end(),
            0,
            [](int total, const music& music_item)
            {
                return total + music_item.get_duration();
            });

        std::cout << "\n===== STL =====\n";
        std::cout << "Music over limit: " << long_musics << '\n';
        std::cout << "Total duration: " << total_duration << " s\n";

        // Q3(C)(D): cálculos independentes em async e agregação compartilhada protegida.
        const int serial_audience = std::accumulate(
            station.get_programs().begin(),
            station.get_programs().end(),
            0,
            [](int total, const auto& program)
            {
                return total + program->calculate_audience();
            });
        const int parallel_audience_total = parallel_audience(station.get_programs());

        std::cout << "Serial audience total: " << serial_audience << '\n';
        std::cout << "Parallel audience total: " << parallel_audience_total << '\n';

        // Q4(D): DIP com repositório em memória, sem efeito colateral de arquivo.
        auto memory_repository = make_memory_repository();
        station_service service(*memory_repository);
        service.save(station);
        radio_station restored = service.load();

        std::cout << "\n===== DIP =====\n";
        std::cout << "Restored station: " << restored.get_name() << '\n';

        // Q4(A)(B): repositório JSON de produção.
        auto json_repository = make_json_repository("state.json");
        station_service json_service(*json_repository);
        json_service.save(station);
        radio_station loaded = json_service.load();
        std::cout << "JSON loaded station: " << loaded.get_name() << '\n';

        const broadcast_content* best = highest_audience(station.get_programs());
        if (best)
        {
            std::cout << "\n===== HIGHEST AUDIENCE =====\n\n";
            best->display();
        }

        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Fatal error: " << error.what() << '\n';
        return 1;
    }
}
