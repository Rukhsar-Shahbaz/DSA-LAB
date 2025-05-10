#include <iostream>
#include <string>
#include <stack>
#include <cctype>

#define MAX_ATTEMPTS 6

using namespace std;

struct Player {
    string name;
    int score;

    Player(string playerName = "") {
        name = playerName;
        score = 0;
    }
};

struct Node {
    char letter;
    Node* next;
    Node(char ch) {
        letter = ch;
        next = NULL;
    }
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() {
        head = NULL;
    }

    void insert(char ch) {
        Node* newNode = new Node(ch);
        if (head == NULL) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != NULL)
                temp = temp->next;
            temp->next = newNode;
        }
    }

    bool contains(char ch) {
        Node* temp = head;
        while (temp != NULL) {
            if (temp->letter == ch)
                return true;
            temp = temp->next;
        }
        return false;
    }

    void display() {
        Node* temp = head;
        while (temp != NULL) {
            cout << temp->letter << " ";
            temp = temp->next;
        }
    }

    ~LinkedList() {
        Node* current = head;
        while (current != NULL) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
};
void drawHangman(int attemptsLeft) {
    if (attemptsLeft == 5) {
        cout << "   _____\n";
        cout << "  |     |\n";
        cout << "  |     O\n";
        cout << "  |\n";
        cout << "  |\n";
        cout << "  |\n";
    }
    else if (attemptsLeft == 4) {
        cout << "v   _____\n";
        cout << "  |     |\n";
        cout << "  |     O\n";
        cout << "  |     |\n";
        cout << "  |\n";
        cout << "  |\n";
    }
    else if (attemptsLeft == 3) {
        cout << "   _____\n";
        cout << "  |     |\n";
        cout << "  |     O\n";
        cout << "  |    /|\n";
        cout << "  |\n";
        cout << "  |\n";
    }
    else if (attemptsLeft == 2) {
        cout << "   _____\n";
        cout << "  |     |\n";
        cout << "  |     O\n";
        cout << "  |    /|\\\n";
        cout << "  |\n";
        cout << "  |\n";
    }
    else if (attemptsLeft == 1) {
        cout << "   _____\n";
        cout << "  |     |\n";
        cout << "  |     O\n";
        cout << "  |    /|\\\n";
        cout << "  |    /\n";
        cout << "  |\n";
    }
    else if (attemptsLeft == 0) {
        cout << "   _____\n";
        cout << "  |     |\n";
        cout << "  |     O\n";
        cout << "  |    /|\\\n";
        cout << "  |    / \\\n";
        cout << "  |\n";
    }
}
bool stackContains(stack<char> s, char ch) {
    while (!s.empty()) {
        if (s.top() == ch) return true;
        s.pop();
    }
    return false;
}

void inputPlayerNames(Player& setter, Player& guesser) {
    cout << "Enter name of the word setter: ";
    cin.ignore();
    getline(cin, setter.name);

    while (setter.name.empty()) {
        cout << "Name cannot be empty. Please enter again: ";
        getline(cin, setter.name);
    }

    bool validSetter = false;
    while (!validSetter) {
        validSetter = true;
        for (int i = 0; i < setter.name.length(); ++i) {
            if (!isalpha(setter.name[i]) && !isspace(setter.name[i])) {
                validSetter = false;
                break;
            }
        }

        if (!validSetter) {
            cout << "Invalid name. Only alphabet letters and spaces are allowed. Please try again: ";
            getline(cin, setter.name);
        }
    }

    cout << "Enter name of the guesser: ";
    getline(cin, guesser.name);

    while (guesser.name.empty()) {
        cout << "Name cannot be empty. Please enter again: ";
        getline(cin, guesser.name);
    }

    bool validGuesser = false;
    while (!validGuesser) {
        validGuesser = true;
        for (int i = 0; i < guesser.name.length(); ++i) {
            if (!isalpha(guesser.name[i]) && !isspace(guesser.name[i])) {
                validGuesser = false;
                break;
            }
        }

        if (!validGuesser) {
            cout << "Invalid name. Only alphabet letters and spaces are allowed. Please try again: ";
            getline(cin, guesser.name);
        }
    }
}

string getSecretWordFromPlayer(string setterName) {
    string word;
    bool valid = false;

    while (!valid) {
        cout << setterName << ", enter the secret word (only alphabets, no spaces): ";
        getline(cin, word);

        valid = true;
        for (int i = 0; i < word.length(); ++i) {
            if (!isalpha(word[i])) {
                cout << "Error: Word must contain only letters and no spaces. Try again.\n";
                valid = false;
                break;
            }
        }
    }

    return word;
}

string maskSecretWord(string word) {
    return string(word.length(), '_');
}

void displayWrongGuesses(stack<char> s) {
    cout << "Wrong Guesses: ";
    stack<char> temp;
    while (!s.empty()) {
        temp.push(s.top());
        s.pop();
    }
    while (!temp.empty()) {
        cout << temp.top() << " ";
        temp.pop();
    }
    cout << endl;
}

void displayStatus(string currentWord, int attemptsLeft, LinkedList& guessedLetters, stack<char>& wrongGuesses) {
    cout << "\nWord: " << currentWord << endl;
    cout << "Attempts Left: " << attemptsLeft << endl;
    cout << "Guessed Letters: ";
    guessedLetters.display();
    cout << endl;
    displayWrongGuesses(wrongGuesses);
}

bool processGuess(char guess, string& secretWord, string& currentWord) {
    bool found = false;
    for (int i = 0; i < secretWord.length(); ++i) {
        if (tolower(secretWord[i]) == guess) {
            currentWord[i] = secretWord[i];
            found = true;
        }
    }
    return found;
}

bool playRound(Player& setter, Player& guesser) {
    string secretWord = getSecretWordFromPlayer(setter.name);
    string currentWord = maskSecretWord(secretWord);
    int attemptsLeft = MAX_ATTEMPTS;
    LinkedList guessedLetters;
    stack<char> wrongGuesses;

    while (attemptsLeft > 0) {
        displayStatus(currentWord, attemptsLeft, guessedLetters, wrongGuesses);

        string input;
        cout << guesser.name << ", enter a letter: ";
        cin >> input;

        if (input.length() != 1 || !isalpha(input[0])) {
            cout << "Invalid input! Try again.\n";
            continue;
        }

        char guess = tolower(input[0]);

        if (guessedLetters.contains(guess) || stackContains(wrongGuesses, guess)) {
            cout << "Already guessed!\n";
            continue;
        }

        if (processGuess(guess, secretWord, currentWord)) {
            cout << "Correct!\n";
            guessedLetters.insert(guess);
        } else {
            cout << "Wrong!\n";
            wrongGuesses.push(guess);
            attemptsLeft--;
            drawHangman(attemptsLeft);
        }

        if (currentWord == secretWord) {
            cout << "\nCongratulations, " << guesser.name << "! You guessed the word: " << secretWord << endl;
            guesser.score += 10;
            return true;
        }
    }

    cout << guesser.name << " failed to guess the word: " << secretWord << endl;
    return false;
}

void showScoreboard(Player p1, Player p2) {
    cout << "\n--- Scoreboard ---\n";
    cout << p1.name << ": " << p1.score << " points\n";
    cout << p2.name << ": " << p2.score << " points\n";

    if (p1.score > p2.score)
        cout << "\n  " << p1.name << " is leading!\n";
    else if (p2.score > p1.score)
        cout << "\n  " << p2.name << " is leading!\n";
    else
        cout << "\nIt's a tie!\n";
}

void swapPlayers(Player& a, Player& b) {
    Player temp = a;
    a = b;
    b = temp;
}

int main() {
    cout << "======================================" << endl;
    cout << "       Welcome to Hangman Game!       " << endl;
    cout << "======================================" << endl;
    cout << "This is a two-player game." << endl;
    cout << "One player will set the word, and the other will try to guess it." << endl;
    cout << "Each correct guess gives points. Let's begin!\n" << endl;

    Player player1, player2;
    inputPlayerNames(player1, player2);

    char choice;
    do {
        playRound(player1, player2);
        swapPlayers(player1, player2);
        showScoreboard(player1, player2);

        cout << "\nDo you want to play another round? (y/n): ";
        cin >> choice;
        cin.ignore();

    } while (tolower(choice) == 'y');

    cout << "\nThanks for playing! Final Results:\n";
    showScoreboard(player1, player2);

    return 0;
}

