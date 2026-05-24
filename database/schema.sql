/* parte de Pedro */

CREATE DATABASE xadrez;

USE xadrez;

CREATE TABLE jogadores(

    id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(50),
    vitorias INT DEFAULT 0

);

CREATE TABLE partidas(

    id INT AUTO_INCREMENT PRIMARY KEY,

    jogador1 INT,
    jogador2 INT,

    vencedor INT,

    data_partida TIMESTAMP
    DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY(jogador1)
    REFERENCES jogadores(id),

    FOREIGN KEY(jogador2)
    REFERENCES jogadores(id),

    FOREIGN KEY(vencedor)
    REFERENCES jogadores(id)

);

CREATE TABLE movimentos(

    id INT AUTO_INCREMENT PRIMARY KEY,

    partida_id INT,

    jogada VARCHAR(20),

    FOREIGN KEY(partida_id)
    REFERENCES partidas(id)

);