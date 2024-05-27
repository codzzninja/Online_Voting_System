#include <stdio.h>
#include <string.h>

struct Voter {
    char username[50];
    char password[50];
    int voted;
};

struct Party {
    char symbol;
    char leader[50];
    int votes;
};

char adminUsername[] = "admin";
char adminPassword[] = "adminpass";
struct Voter voters[100];
struct Party parties[10];
int numVoters = 0;
int numParties = 0;

void adminLogin();
void addVoter();
void addParty();
void viewVotes();
void viewVoters();
void voterLogin();
void vote();
void viewResults();
void saveDataToFile();
void loadDataFromFile();
int isUsernameUnique(char username[]);
int isSymbolUnique(char symbol);
void conductTieBreakerElection(char tiedSymbols[], int numTied);

int main() {
    loadDataFromFile();
    
    int choice;

    do {
        printf("\n=== ONLINE VOTING SYSTEM===\n");
        printf("\n===WELCOME TO ONLINE VOTING SYSTEM===\n");
        printf("\n===Thank you for choosing this platform===\n");
        printf("1. Admin Panel\n");
        printf("2. Voter Panel\n");
        printf("3. View Election Results\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                voterLogin();
                break;
            case 3:
                viewResults();
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
    
    saveDataToFile();

    return 0;
}

void adminLogin() {
    char username[50], password[50];

    printf("\nEnter admin username: ");
    scanf("%s", username);
    printf("Enter admin password: ");
    scanf("%s", password);

    if (strcmp(username, adminUsername) == 0 && strcmp(password, adminPassword) == 0) {
        printf("Admin login successful.\n");

        int choice;
        do {
            printf("\nAdmin Panel\n");
            printf("1. Add Voter\n");
            printf("2. Add Party\n");
            printf("3. View Votes\n");
            printf("4. View Voters\n");
            printf("5. Exit Admin Panel\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    addVoter();
                    break;
                case 2:
                    addParty();
                    break;
                case 3:
                    viewVotes();
                    break;
                case 4:
                    viewVoters();
                    break;
                case 5:
                    printf("Exiting Admin Panel...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 5);
    } else {
        printf("Invalid admin credentials.\n");
    }
}

void addVoter() {
    char username[50], password[50];

    printf("\nEnter voter username: ");
    scanf("%s", username);

    // Check for unique username
    while (!isUsernameUnique(username)) {
        printf("Username already exists. Please enter a different username: ");
        scanf("%s", username);
    }

    printf("Enter voter password: ");
    scanf("%s", password);

    strcpy(voters[numVoters].username, username);
    strcpy(voters[numVoters].password, password);
    voters[numVoters].voted = 0;
    numVoters++;

    printf("Voter added successfully.\n");
}

void addParty() {
    char symbol;
    char leader[50];

    printf("\nEnter party symbol: ");
    scanf(" %c", &symbol);

    // Check for unique symbol
    while (!isSymbolUnique(symbol)) {
        printf("Party symbol already exists. Please enter a different symbol: ");
        scanf(" %c", &symbol);
    }

    printf("Enter party leader name: ");
    scanf("%s", leader);

    parties[numParties].symbol = symbol;
    strcpy(parties[numParties].leader, leader);
    parties[numParties].votes = 0;
    numParties++;

    printf("Party added successfully.\n");
}

void viewVotes() {
    printf("\nParty\tVotes\n");
    for (int i = 0; i < numParties; i++) {
        printf("%c\t%d\n", parties[i].symbol, parties[i].votes);
    }
}

void viewVoters() {
    printf("\nVoters who have voted:\n");
    for (int i = 0; i < numVoters; i++) {
        if (voters[i].voted) {
            printf("%s\n", voters[i].username);
        }
    }

    printf("\nVoters who have not voted:\n");
    for (int i = 0; i < numVoters; i++) {
        if (!voters[i].voted) {
            printf("%s\n", voters[i].username);
        }
    }
}

void vote(int voterIndex) {
    char voteSymbol;

    printf("\nVote for one of the following parties:\n");
    for (int i = 0; i < numParties; i++) {
        printf("%c - %s\n", parties[i].symbol, parties[i].leader);
    }

    printf("Enter the symbol of the party you want to vote for: ");
    scanf(" %c", &voteSymbol);

    int partyIndex = -1;
    for (int i = 0; i < numParties; i++) {
        if (parties[i].symbol == voteSymbol) {
            partyIndex = i;
            break;
        }
    }

    if (partyIndex != -1) {
        parties[partyIndex].votes++;
        voters[voterIndex].voted = 1;
        printf("Your vote has been recorded. Thank you for voting!\n");
    } else {
        printf("Invalid party symbol. Vote not recorded.\n");
    }
}

void voterLogin() {
    char username[50], password[50];
    int found = 0, index;

    printf("\nEnter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    for (int i = 0; i < numVoters; i++) {
        if (strcmp(username, voters[i].username) == 0 && strcmp(password, voters[i].password) == 0) {
            if (!voters[i].voted) {
                found = 1;
                index = i;
                break;
            } else {
                printf("You have already voted.\n");
                return;
            }
        }
    }

    if (found) {
        printf("Login successful. You can now vote.\n");
        vote(index);
    } else {
        printf("Invalid username or password.\n");
    }
}

void viewResults() {
    printf("\nElection Results\n");
    printf("Party Symbol | Party Leader | Votes\n");
    for (int i = 0; i < numParties; i++) {
        printf("     %c       |   %s   |   %d\n", parties[i].symbol, parties[i].leader, parties[i].votes);
    }

    int maxVotes = 0;
    char winner[50];
    int tie = 0;
    char tiedSymbols[10];
    int numTied = 0;

    for (int i = 0; i < numParties; i++) {
        if (parties[i].votes > maxVotes) {
            maxVotes = parties[i].votes;
            strcpy(winner, parties[i].leader);
            tie = 0;
            numTied = 1;
            tiedSymbols[0] = parties[i].symbol;
        } else if (parties[i].votes == maxVotes) {
            tie = 1;
            tiedSymbols[numTied] = parties[i].symbol;
            numTied++;
        }
    }

    if (tie) {
        printf("\nThere is a tie between the following parties:\n");
        for (int i = 0; i < numTied; i++) {
            for (int j = 0; j < numParties; j++) {
                if (parties[j].symbol == tiedSymbols[i]) {
                    printf("%c - %s\n", parties[j].symbol, parties[j].leader);
                    break;  // Break the inner loop once the tied party is found
                }
            }
        }
        printf("A tie-breaking election will be conducted.\n");
        conductTieBreakerElection(tiedSymbols, numTied);
    } else {
        printf("\nWinner of the election: %s\n", winner);
    }
}

void conductTieBreakerElection(char tiedSymbols[], int numTied) {
    // Reset votes for tied parties
    for (int i = 0; i < numParties; i++) {
        for (int j = 0; j < numTied; j++) {
            if (parties[i].symbol == tiedSymbols[j]) {
                parties[i].votes = 0;
            }
        }
    }

    // Reset the voted status for all voters
    for (int i = 0; i < numVoters; i++) {
        voters[i].voted = 0;
    }

    // Display tied parties for voting
    printf("\nA tie-breaking election will be conducted. Please vote for one of the following tied parties:\n");
    for (int i = 0; i < numTied; i++) {
        for (int j = 0; j < numParties; j++) {
            if (parties[j].symbol == tiedSymbols[i]) {
                printf("%c - %s\n", parties[j].symbol, parties[j].leader);
                break;  // Break the inner loop once the tied party is found
            }
        }
    }

    // Conduct tie-breaking election
    for (int i = 0; i < numVoters; i++) {
        printf("\nVoter %s, please vote again:\n", voters[i].username);
        for (int i = 0; i < numTied; i++) {
            for (int j = 0; j < numParties; j++) {
                if (parties[j].symbol == tiedSymbols[i]) {
                    printf("%c - %s\n", parties[j].symbol, parties[j].leader);
                    break;  // Break the inner loop once the tied party is found
                }
            }
        }
        char voteSymbol;
        printf("Enter the symbol of the party you want to vote for: ");
        scanf(" %c", &voteSymbol);

        int partyIndex = -1;
        for (int i = 0; i < numParties; i++) {
            if (parties[i].symbol == voteSymbol) {
                partyIndex = i;
                break;
            }
        }

        if (partyIndex != -1) {
            parties[partyIndex].votes++;
            voters[i].voted = 1;
            printf("Your vote has been recorded. Thank you for voting!\n");
        } else {
            printf("Invalid party symbol. Vote not recorded.\n");
        }
    }

    // Display tie-breaking election results
    printf("\nTie-Breaker Election Results\n");
    printf("Party Symbol | Party Leader | Votes\n");
    for (int i = 0; i < numTied; i++) {
        for (int j = 0; j < numParties; j++) {
            if (parties[j].symbol == tiedSymbols[i]) {
                printf("     %c       |   %s   |   %d\n", parties[j].symbol, parties[j].leader, parties[j].votes);
                break;  // Break the inner loop once the tied party is found
            }
        }
    }
    viewResults();
}

void saveDataToFile() {
    FILE *file = fopen("voting_data.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numVoters; i++) {
        fprintf(file, "%s,%s,%d\n", voters[i].username, voters[i].password, voters[i].voted);
    }

    for (int i = 0; i < numParties; i++) {
        fprintf(file, "%c,%s,%d\n", parties[i].symbol, parties[i].leader, parties[i].votes);
    }

    fclose(file);
    printf("Data saved to file successfully.\n");
}

void loadDataFromFile() {
    FILE *file = fopen("voting_data.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    while (!feof(file)) {
        char username[50], password[50];
        int voted;
        fscanf(file, "%[^,],%[^,],%d\n", username, password, &voted);
        if (feof(file)) break;
        strcpy(voters[numVoters].username, username);
        strcpy(voters[numVoters].password, password);
        voters[numVoters].voted = voted;
        numVoters++;
    }

    while (!feof(file)) {
        char leader[50];
        char symbol;
        int votes;
        fscanf(file, " %c,%[^,],%d\n", &symbol, leader, &votes);
        if (feof(file)) break;
        parties[numParties].symbol = symbol;
        strcpy(parties[numParties].leader, leader);
        parties[numParties].votes = votes;
        numParties++;
    }

    fclose(file);
    printf("Data loaded from file successfully.\n");
}

int isUsernameUnique(char username[]) {
    for (int i = 0; i < numVoters; i++) {
        if (strcmp(username, voters[i].username) == 0) {
            return 0;
        }
    }
    return 1;
}

int isSymbolUnique(char symbol) {
    for (int i = 0; i < numParties; i++) {
        if (symbol == parties[i].symbol) {
            return 0;
        }
    }
    return 1;
}