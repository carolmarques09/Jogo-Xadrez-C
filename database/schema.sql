/* parte de Pedro */

CREATE DATABASE IF NOT EXISTS xadrez;

USE xadrez;

CREATE TABLE IF NOT EXISTS jogadores(

    id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(50) NOT NULL UNIQUE,
    vitorias INT DEFAULT 0

);

CREATE TABLE IF NOT EXISTS partidas(

    id INT AUTO_INCREMENT PRIMARY KEY,

    jogador1 INT NOT NULL,
    jogador2 INT NOT NULL,

    vencedor INT NULL,

    data_partida TIMESTAMP
    DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY(jogador1)
    REFERENCES jogadores(id),

    FOREIGN KEY(jogador2)
    REFERENCES jogadores(id),

    FOREIGN KEY(vencedor)
    REFERENCES jogadores(id)

);

CREATE TABLE IF NOT EXISTS movimentos(

    id INT AUTO_INCREMENT PRIMARY KEY,

    partida_id INT NOT NULL,

    jogada VARCHAR(20) NOT NULL,

    FOREIGN KEY(partida_id)
    REFERENCES partidas(id)

);
