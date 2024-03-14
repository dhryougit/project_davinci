#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

enum class TileColor { WHITE, BLACK };
enum class TileStatus { HIDDEN, REVEALED };

struct Tile {
    int number;
    TileColor color;
    TileStatus status;
};

class Player {
private:
    vector<Tile> tiles;
public:
    void addTile(Tile tile) {
        tiles.push_back(tile);
    }

    void displayTiles() {
        sortTiles();
        cout << "Player's Tiles: [ ";
        for (size_t i = 0; i < tiles.size(); ++i) {
            cout << tiles[i].number << (tiles[i].color == TileColor::WHITE ? "w" : "b") << "(" 
                 << (tiles[i].status == TileStatus::HIDDEN ? "hidden" : "revealed") << ")";
            if (i != tiles.size() - 1)
                cout << ", ";
        }
        cout << " ]" << endl;
    }

    bool isTileRevealed(int index) {
        return tiles[index].status == TileStatus::REVEALED;
    }

    Tile& getTile(int index) {
        return tiles[index];
    }

    int getNumTiles() {
        return tiles.size();
    }

    void sortTiles() {
        sort(tiles.begin(), tiles.end(), [](const Tile& a, const Tile& b) {
            if (a.number != b.number) {
                return a.number < b.number;
            } else {
                return a.color == TileColor::BLACK;
            }
        });
    }

 

    bool allTilesRevealed() {
        for (const auto& tile : tiles) {
            if (tile.status == TileStatus::HIDDEN) {
                return false;
            }
        }
        return true;
    }
};

class DaVinciCodeGame {
private:
    vector<Tile> centerTiles;
    vector<Player> players;
    int currentPlayerIndex;
public:
    DaVinciCodeGame(int numPlayers) {
        currentPlayerIndex = 0;
        initializeTiles();
        shuffleTiles();
        distributeTiles(numPlayers);
    }

    void initializeTiles() {
        for (int i = 1; i <= 13; ++i) {
            centerTiles.push_back({i, TileColor::WHITE, TileStatus::HIDDEN});
            centerTiles.push_back({i, TileColor::BLACK, TileStatus::HIDDEN});
        }
    }

    void shuffleTiles() {
        random_shuffle(centerTiles.begin(), centerTiles.end());
    }

    void distributeTiles(int numPlayers) {
        for (int i = 0; i < numPlayers; ++i) {
            Player player;
            for (int j = 0; j < 4; ++j) {
                player.addTile(centerTiles.back());
                centerTiles.pop_back();
            }
            players.push_back(player);
        }
    }

    void displayPlayersTiles() {
        for (int i = 0; i < players.size(); ++i) {
            cout << "Player " << i + 1 << ": ";
            players[i].displayTiles();
            cout << endl;
        }
    }

    bool makeGuess(int targetPlayerIndex, int targetTileIndex, int guessedNumber) {
        Tile& targetTile = players[targetPlayerIndex].getTile(targetTileIndex);
        if (targetTile.number == guessedNumber) {
            targetTile.status = TileStatus::REVEALED;
            return true;
        } else {
            return false;
        }
    }

    void play() {
        bool correctGuess = false;
        while (true) {

            displayPlayersTiles();
            
            cout << "Player " << currentPlayerIndex + 1 << "'s Turn: \n" << endl;
            
            Player& currentPlayer = players[currentPlayerIndex];
            
            if (!currentPlayer.allTilesRevealed() && !correctGuess) {
                Tile& clueTile =centerTiles.back();
                currentPlayer.addTile(clueTile);
                cout << "Clue tile received: " << clueTile.number << (clueTile.color == TileColor::WHITE ? "w" : "b" )<<endl;
                
                centerTiles.pop_back();
                
                currentPlayer.displayTiles();
            }
            correctGuess = false;
            int targetPlayerIndex, targetTileIndex, guessedNumber;
            // cout << "Enter target player index (1-" << players.size() << "): ";
            // cin >> targetPlayerIndex;
            targetPlayerIndex = (currentPlayerIndex + 1) % players.size() ;
            Player& targetPlayer = players[targetPlayerIndex];
            cout << "Enter target tile index (1-" << targetPlayer.getNumTiles() << "): ";
            cin >> targetTileIndex;
            cout << "Enter guessed number: ";
            cin >> guessedNumber;

            if (makeGuess(targetPlayerIndex , targetTileIndex - 1, guessedNumber)) {
                cout << "Correct guess! Player " << currentPlayerIndex + 1 << " gets another turn. \n" << endl;
                correctGuess = true;
            } else {
                cout << "Incorrect guess! Player " << currentPlayerIndex + 1 << " reveals a clue tile. \n" << endl;
                // currentPlayer.displayTiles();
                // Reveal the last received clue tile
                if (!currentPlayer.allTilesRevealed()) {
                    currentPlayer.getTile(currentPlayer.getNumTiles() - 1).status = TileStatus::REVEALED;
                  
                    // currentPlayer.displayTiles();
                }
            }

            if (targetPlayer.allTilesRevealed()) {
                cout << "Player " << targetPlayerIndex + 1 << " has lost!" << endl;
                cout << "Player " << currentPlayerIndex + 1 << " is the winner!" << endl;
                 return;
                
                // players.erase(players.begin() + currentPlayerIndex);
                // if (players.size() == 1) {
                //     cout << "Player " << players[0].getNumTiles() << " is the winner!" << endl;
                //     return;
                // }
            }

            if (!correctGuess) {
                currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
            }
        }
    }
};

int main() {
    srand(time(nullptr));
    int numPlayers;
    // cout << "Enter number of players: ";
    // cin >> numPlayers;
    DaVinciCodeGame game(2);
    game.play();
    return 0;
}