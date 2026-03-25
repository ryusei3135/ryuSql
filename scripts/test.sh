make clean;
make;
DIR="$(cd "$(dirname "$0")" && pwd)"
APP="$DIR/../db"
$APP "CREATE TABLE name(j INT)"
$APP "SELECT name FROM table"
$APP "INSERT INTO (a, b) VALUES (HE, E)"