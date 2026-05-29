# Jogo de Xadrez em C

## Compilar

```cmd
cd C:\Users\Lenovo\Documents\jogo_xadrez_c
make
```

Requisitos: **w64devkit** (gcc/make), **Raylib** em `C:\raylib`.

## Executar

```cmd
bin\chess.exe
```

Abre uma janela com o tabuleiro. Clique na peça de origem e depois no destino. **R** reinicia; **ESC** fecha.

---

## MySQL (passo a passo)

### 1. Instalar o connector C

1. Descarregar [MySQL Connector/C](https://dev.mysql.com/downloads/connector/c/) (Windows, 64-bit).
2. Extrair para `C:\mysql` com esta estrutura:
   - `C:\mysql\include\mysql\mysql.h`
   - `C:\mysql\lib\libmysqlclient.a` (MinGW) ou `libmysqlclient.lib` (MSVC)

### 2. Servidor MySQL

Instalar [MySQL Server](https://dev.mysql.com/downloads/mysql/) ou usar XAMPP/WAMP. O serviço deve estar a correr na porta **3306**.

### 3. Criar a base de dados

```cmd
mysql -u root -p < database\schema.sql
```

### 4. Configurar credenciais

```cmd
copy database\config.env.example database\config.env
```

Editar `database\config.env` com o teu utilizador e palavra-passe.

### 5. Recompilar

```cmd
make clean
make
```

Se o header existir em `C:\mysql`, a mensagem "compilando sem banco de dados" **deixa de aparecer**.

### 6. Verificar no MySQL

```sql
USE xadrez;
SELECT * FROM partidas ORDER BY id DESC LIMIT 1;
SELECT * FROM movimentos WHERE partida_id = 1;
```

Cada jogada na interface grava uma linha em `movimentos` quando o MySQL está ligado.

---

## Estrutura

| Pasta / ficheiro | Função |
|------------------|--------|
| `src/board.c` | Tabuleiro e peças |
| `src/movement.c` | Notação UCI e `aplicarMovimento` |
| `src/graphics.c` | Interface Raylib |
| `src/game.c` | Fluxo do jogo + MySQL |
| `src/database.c` | Ligação MySQL |
| `database/schema.sql` | Tabelas |
