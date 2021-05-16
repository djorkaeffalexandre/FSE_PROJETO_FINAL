<h1 align="center">
  <img alt="FSE" src="https://i.imgur.com/3Q8FJri.jpg" width="500" height="300" />
  <br>
  FSE
</h1>

# 🔋 Fundamentos de Sistemas Embarcados

Projeto Final da matéria de Fundamentos de Sistemas Embarcados.

| Aluno(a) | Matricula |
| :--: | :--: |
| Djorkaeff Alexandre Vilela Pereira | 16/0026822 |
| Guilherme Siqueira Brandão | 16/0007763 |

## 🎥 Visão Geral

Assista o vídeo de explicação sobre o projeto no [Youtube](https://youtu.be/_CUdlJEPX1c).

## 🏁 Projeto Final

A definição da aplicação pode ser encontrada no [Gitlab da disciplina](https://gitlab.com/fse_fga/projetos_2020_2/trabalho-final-2020-2).

## 🚀 Pré-requisitos 

Os pré requisitos para a construção do projeto são:

- [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [Yarn](https://yarnpkg.com)

## 🛠 Começando

A inicialização da aplicação deverá ser feita de modo a executar primeiramente a aplicação desenvolvida como cliente do projeto, que está dentro da pasta [central](https://github.com/djorkaeffalexandre/FSE_PROJETO_FINAL/tree/main/central).

Para executar o projeto central você deverá ter o gerenciador de pacotes [Yarn](https://yarnpkg.com) instalado em sua máquina.

Como primeiro passo você deverá entrar através do `terminal` na pasta da aplicação utilizando o comando:
```sh
cd central
```
Instalar as dependências utilizando:
```sh
yarn install
```
E executar a construção do projeto, que ficará disponível para acesso no caminho [localhost:3000](localhost:3000).
```sh
yarn start
```

<h1 align="center">
  <img alt="WEB APP" src="https://i.imgur.com/84tGddW.png" width="500" height="300" />
</h1>

✅ Através da aplicação central é possível baixar um `LOG` de registro de todos os eventos realizados pelo usuário em formato `CSV`. ✅

## 📝 Configuração

Para iniciar a aplicação embarcada, será necessária a inicialização do ambiente de desenvolvimento [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/), com ele poderemos configurar, construir e instalar o projeto.

Após a inicialização do ambiente de desenvolvimento [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/) é necessário executar o comando que permite a configuração das variáveis de ambiente da aplicação, através do comando:
```sh
idf.py menuconfig
```

A configuração do WiFi pode ser feita através do menu `Wifi Configuration`.

<h1 align="center">
  <img alt="Wifi Configuration" src="https://i.imgur.com/1KCyZJq.png" width="500" height="300" />
</h1>

<h1 align="center">
  <img alt="Wifi Configuration" src="https://i.imgur.com/4hpQbSS.png" width="500" height="300" />
</h1>

Deverá ser realizada também a configuração das variáveis do projeto utilizadas para comunicação e utilização ou não de sensores a depender do modelo de alimentação utilizado.

Utilizando o menu `FSE Configuration` é possível selecionar o modelo de alimentação da placa e a configuração de um identificador do aluno que está utilizando o projeto.

:warning: Deverá ser utilizada a matrícula 160026822 para conexão com o projeto `central`, ou alterar a configuração padrão da aplicação cliente no [sistema de serviço](https://github.com/djorkaeffalexandre/FSE_PROJETO_FINAL/blob/main/central/src/Server.js#L3). :warning:

<h1 align="center">
  <img alt="FSE Configuration" src="https://i.imgur.com/xn14lpS.png" width="500" height="300" />
</h1>

<h1 align="center">
  <img alt="FSE Configuration" src="https://i.imgur.com/vvXfUFN.png" width="500" height="300" />
</h1>

Após as configurações, para construir o projeto deverá ser utilizado o comando padrão de construção da [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/).
```sh
idf.py build
```

Após a construção da aplicação podemos executá-la na porta de conexão através do comando:
```sh
idf.py -p (PORT) flash
```

Sendo (PORT) a representação da porta onde o componente está conectado.
