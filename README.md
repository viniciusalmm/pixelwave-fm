# PixelWave FM

Sistema de rádio online desenvolvido para a disciplina de **Programação Orientada a Objetos (POO)** da **Universidade Federal da Paraíba (UFPB)**. O repositório evolui o domínio dos TP1 e TP2 e incorpora os requisitos do **Trabalho Prático 3 (TP3)**.

## Tecnologias

- C++20
- CMake
- STL
- `std::unique_ptr`
- `std::optional` e `std::variant`
- `std::thread`/`std::async`, `std::future` e `std::mutex`
- `nlohmann/json`
- Catch2 v3 para os testes extras
- Qt 6 Widgets para a interface gráfica extra

## Estrutura

```text
pixelwave-fm/
├── CMakeLists.txt
├── README.md
├── src/
│   ├── main.cpp
│   ├── music.hpp / music.cpp
│   ├── playlist.hpp / playlist.cpp
│   ├── listener.hpp / listener.cpp
│   ├── radio_station.hpp / radio_station.cpp
│   ├── broadcast_content.hpp / broadcast_content.cpp
│   ├── music_program.hpp / music_program.cpp
│   ├── news_program.hpp / news_program.cpp
│   ├── transmissible.hpp
│   ├── generic.hpp
│   ├── errors.hpp
│   ├── utils.hpp
│   ├── json_serialization.hpp / json_serialization.cpp
│   ├── repository.hpp / repository.cpp
│   ├── service.hpp
│   ├── demo_data.hpp / demo_data.cpp
│   ├── main_window.hpp / main_window.cpp
│   └── gui_main.cpp
└── tests/
    ├── test_tp3.cpp
    └── concept_violation.cpp
```

## Build

O projeto exige C++20.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/pixelwave_fm
```

Em geradores multi-configuração, como alguns ambientes do Visual Studio, execute o binário dentro da configuração gerada, por exemplo `build/Debug/`.

A interface Qt é um alvo extra e fica desativada no build padrão para que a parte obrigatória continue compilando mesmo em máquinas sem Qt. Para construir a GUI, é necessário ter **Qt 6 com o componente Widgets** instalado:

```bash
cmake -S . -B build-gui -DCMAKE_BUILD_TYPE=Debug -Denable_qt_gui=ON -DBUILD_TESTING=OFF
cmake --build build-gui
./build-gui/pixelwave_gui
```

Em geradores multi-configuração, o executável pode estar em `build-gui/Debug/pixelwave_gui`.

## Programação Genérica

### Template reutilizável

`registry<item_type>` abstrai um contêiner tipado reutilizável para objetos do domínio. Além de inserção, acesso indexado e tamanho, ele oferece `find_first()` parametrizado por predicado, centralizando uma busca genérica sem expor o `std::vector` interno. No `main()` ele é instanciado com dois tipos distintos: `music_program` e `news_program`.

### CRTP

`counted<derived_type>` adiciona contagem estática de instâncias a `music_program` e `news_program`. O CRTP foi usado porque essa contagem não precisa de despacho dinâmico nem de uma nova vtable; o comportamento é resolvido estaticamente para cada tipo derivado.

### Concept

O concept `calculable` exige que o tipo ofereça `calculate_audience() const` com resultado conversível para `int`. A função `total_audience()` aplica o concept diretamente na assinatura.

A violação pode ser demonstrada sem quebrar o build normal:

```bash
g++ -std=c++20 -Isrc tests/concept_violation.cpp -o concept_violation
```

Esse comando deve falhar porque `int` não satisfaz `calculable`, produzindo diagnóstico de restrição/concept em vez de uma cadeia obscura de erros de template. O arquivo `tests/concept_violation.cpp` é propositalmente excluído do build normal do CMake.

### Ranges

Antes, o filtro por audiência e a transformação para títulos exigiriam etapas explícitas em um laço. No `main()` o processamento é expresso como um pipeline C++20 com dois adaptadores encadeados:

```cpp
auto names = station.get_programs()
    | std::views::filter(/* audiência mínima */)
    | std::views::transform(/* título */);
```

## Tratamento de Erros

A hierarquia parte de `domain_error`, derivada de `std::runtime_error`, e contém exceções específicas como `invalid_music` e `invalid_program`. Elas são lançadas em validações reais do domínio: música inválida e tentativa de adicionar programa nulo.

A busca de ouvinte `find_listener()` retorna `std::optional<listener>` e usa `std::nullopt` quando o apelido não é encontrado. O `main()` demonstra os dois casos.

A busca de música usa `music_search_result`, definido como `std::variant<music, std::string>`, e o resultado é tratado com `std::visit`. O `main()` demonstra tanto o caso de música encontrada quanto o caso de mensagem de erro.

O `main()` também demonstra uma exceção específica sendo capturada pela base `domain_error`.

## STL e Concorrência

Dois contêineres adicionais são usados no domínio:

- `std::map<std::string, int>`: índice ordenado de duração por título da música;
- `std::unordered_set<std::string>`: mantém países de ouvintes sem duplicação, com busca média constante.

Os algoritmos STL demonstrados incluem `std::sort`, `std::count_if`, `std::accumulate`, `std::find_if` e `std::max_element`. A chamada de `std::count_if` usa uma lambda com captura de `duration_limit`.

`parallel_audience()` executa com `std::async(std::launch::async, ...)` o cálculo de audiência de cada programa. Esses cálculos são independentes entre si. A soma `total` é um estado compartilhado entre as tarefas assíncronas e cada atualização ocorre dentro de uma região crítica protegida por `std::mutex` e `std::lock_guard`. Todos os `std::future<void>` são coletados com `future::get()` antes do retorno. O `main()` também calcula a mesma soma de forma serial para permitir a comparação direta dos resultados.

### ThreadSanitizer

Para verificar corrida de dados com GCC ou Clang:

```bash
cmake -S . -B build-tsan -DCMAKE_BUILD_TYPE=Debug -Denable_thread_sanitizer=ON -DBUILD_TESTING=OFF
cmake --build build-tsan
./build-tsan/pixelwave_fm
```

A verificação é considerada limpa quando a execução termina sem mensagens `WARNING: ThreadSanitizer: data race`. O resultado paralelo também deve ser igual à soma serial das audiências dos mesmos programas.

### AddressSanitizer e UndefinedBehaviorSanitizer

Para executar a verificação de memória e comportamento indefinido sugerida no roteiro manual:

```bash
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug -Denable_address_sanitizer=ON -DBUILD_TESTING=OFF
cmake --build build-asan
./build-asan/pixelwave_fm
```

A execução deve terminar sem mensagens `ERROR: AddressSanitizer` e sem diagnósticos do UndefinedBehaviorSanitizer. Use um diretório de build separado do ThreadSanitizer; os dois modos não são ativados simultaneamente neste CMake.

## Serialização

A serialização usa `nlohmann/json` e funções não intrusivas `to_json`/`from_json` para `music`, `listener` e para o estado `radio_station`.

O documento raiz contém `"version": 1`. A desserialização verifica esse campo e rejeita versões diferentes. Como o domínio possui uma hierarquia polimórfica, cada programa é salvo com o campo `"type"`, usado para recriar `music_program` ou `news_program`.

`json_repository` persiste o estado em arquivo JSON. `memory_repository` mantém o JSON apenas em memória para permitir testes da mesma lógica sem arquivo e sem rede.

## SOLID

- **SRP:** a persistência foi retirada da classe `radio_station` e concentrada em serialização/repositórios; essa é a refatoração de responsabilidade única aplicada no TP3.
- **OCP:** `broadcast_content` é o ponto de extensão para novos programas por herança e polimorfismo. Um novo tipo pode implementar a interface sem alterar a lógica de execução da estação; a camada de serialização é o ponto explícito que precisa conhecer novos tipos persistíveis.
- **LSP:** `music_program` e `news_program` são consumidos pela estação como `broadcast_content` e preservam as operações virtuais esperadas (`calculate_audience`, `type` e `display`).
- **ISP:** `transmissible` possui somente a operação `transmit()`, evitando impor operações de transmissão a tipos que não precisam dessa capacidade.
- **DIP:** `station_service` depende da abstração `repository&`, recebida por injeção no construtor, e não das implementações concretas.

As duas implementações da abstração são `json_repository` (produção, arquivo JSON) e `memory_repository` (teste, memória).

## Testes Automatizados — Catch2 (extra)

O Catch2 v3 é configurado via `FetchContent`. O arquivo `tests/test_tp3.cpp` contém cinco `TEST_CASE`s cobrindo:

1. template/concept da Q1;
2. exceção capturável pela base e `optional` da Q2;
3. serialização round-trip da Q4;
4. DIP com `memory_repository`, sem acesso a arquivo;
5. comparação entre o cálculo de audiência serial e o paralelo.

Execute:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Interface Gráfica com Qt — extra Q6

A aplicação inclui uma interface **Qt Widgets** separada do executável de console. A janela é uma camada de apresentação: ela reutiliza `radio_station`, `playlist`, `station_service`, `repository` e as funções de cálculo já existentes, sem duplicar a lógica de negócio na classe de janela.

A GUI demonstra o funcionamento da rádio sem reproduzir áudio real. Ela oferece:

- visualização da playlist;
- indicação de “Agora no ar” para a música selecionada;
- inclusão e remoção de músicas;
- tabela com a programação, horários, tipos e audiência calculada pelo domínio;
- resumo de músicas, ouvintes, programas e audiência total;
- botões **Salvar JSON** e **Carregar JSON**, usando `station_service` e `json_repository`.

O arquivo `demo_data.cpp` contém apenas os dados iniciais de demonstração. A janela não cria regras próprias para cálculo, persistência ou validação do domínio; erros lançados pelas classes do domínio são exibidos ao usuário por caixas de mensagem.

### Screenshot da GUI

O projeto possui um modo de captura da própria janela Qt. Depois de compilar a interface, execute na raiz do repositório:

```bash
./build-gui/pixelwave_gui --capture-screenshot
```

O programa cria `docs/pixelwave_gui.png` usando a renderização real da janela e encerra em seguida. Em Windows com gerador multi-configuração, use o caminho correspondente, por exemplo:

```powershell
.\build-gui\Debug\pixelwave_gui.exe --capture-screenshot
```

Depois da captura, inclua a imagem no repositório e mantenha no README:

```markdown
![Interface Qt do PixelWave FM](docs/pixelwave_gui.png)
```

> A imagem deve ser gerada após a execução local do binário Qt. Ela não é pré-fabricada no repositório porque deve representar a janela realmente executada no ambiente de entrega.

## Convenções do TP3

O código próprio utiliza `snake_case`, membros privados terminados em `_`, identificadores em inglês e comentários em português.

## Autor

**Vinícius Alencar de Medeiros**  
Universidade Federal da Paraíba (UFPB)  
Programação Orientada a Objetos — 2026.1
