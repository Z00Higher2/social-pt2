#include <string>
#include <iostream>
using namespace std;

class Profile {
private:
    string username;
    string displayname;

public:
    // Profile constructor for a user (initializing private variables username=usrn, displayname=dspn)
    Profile(string usrn, string dspn);
    // Default Profile constructor (username="", displayname="")
    Profile();
    // Return username
    string getUsername();
    // Return name in the format: "displayname (@username)"
    string getFullName();
    // Change display name
    void setDisplayName(string dspn);
};

// Implementations of Profile class methods

// Profile constructor implementation
Profile::Profile(string usrn, string dspn) {
    username = usrn;
    displayname = dspn;
}

// Default Profile constructor implementation
Profile::Profile() {
    username = "";
    displayname = "";
}

// Return username implementation
string Profile::getUsername() {
    return username;
}

// Return name in the format: "displayname (@username)" implementation
string Profile::getFullName() {
    return displayname + " (@" + username + ")";
}

// Change display name implementation
void Profile::setDisplayName(string dspn) {
    displayname = dspn;
}
struct Post{
    string username;
    string message;
};
class Network {
private:
    static const int MAX_USERS = 20; // max number of user profiles
    int numUsers;                    // number of registered users
    Profile profiles[MAX_USERS];     // user profiles array:
                                     // mapping integer ID -> Profile

    // Returns user ID (index in the 'profiles' array) by their username
    // (or -1 if username is not found)
    int findID(string usrn);
    bool following[MAX_USERS][MAX_USERS];  // friendship matrix:
    // following[id1][id2] == true when id1 is following id2
    static const int MAX_POSTS = 100;
    int numPosts;                    // number of posts
    Post posts[MAX_POSTS];           // array of all posts
public:
    // Constructor, makes an empty network (numUsers = 0)
    Network();
    // Attempts to sign up a new user with specified username and displayname
    // return true if the operation was successful, otherwise return false
    bool addUser(string usrn, string dspn);
     // Make 'usrn1' follow 'usrn2' (if both usernames are in the network).
    // return true if success (if both usernames exist), otherwise return false
    bool follow(string usrn1, string usrn2);

    // Print Dot file (graphical representation of the network)
    void printDot();
    // Add a new post
    bool writePost(string usrn, string msg);
    // Print user's "timeline"
    bool printTimeline(string usrn);
};
bool Network::writePost(string usrn, string msg){
    if(numPosts == MAX_POSTS){
        return false;
    }
    if(findID(usrn) == -1){
        return false;
    }
    Post post;
    post.username = usrn;
    post.message = msg;
    posts[numPosts] = post;
    numPosts++;
    return true;
}
bool Network::printTimeline(string usrn) {
    if (findID(usrn) == -1) {
        return false;
    }

    for (int i = numPosts - 1; i >= 0; i--) {
        if (posts[i].username == usrn || following[findID(usrn)][findID(posts[i].username)]) {
            cout << profiles[findID(posts[i].username)].getFullName() << ": " << posts[i].message << endl;
        }
    }

    return true;
}
bool Network::follow(string usrn1, string usrn2){
    int id1 = findID(usrn1);
    int id2 = findID(usrn2);
    if (id1 == -1 || id2 == -1) return false;
    following[id1][id2] = true;
    return true;
}

void Network::printDot(){
    cout << "digraph {" << endl;
    
    // Print usernames
    for (int i = 0; i < numUsers; i++) {
        cout << "  \"@" << profiles[i].getUsername() << "\"" << endl;
    }

    // Print relationships between users who follow each other
    for (int i = 0; i < numUsers; i++) {
        for (int j = 0; j < numUsers; j++) {
            if (following[i][j]) {
                cout << "  \"@" << profiles[i].getUsername() << "\" -> \"@" << profiles[j].getUsername() << "\"" << endl;
            }
        }
    }
    
    cout << "}" << endl;
}


int Network::findID(string usrn) {
    for (int i = 0; i < numUsers; i++) {
        if (profiles[i].getUsername() == usrn) {
            return i;
        }
    }
    return -1;
}

Network::Network() {
    numUsers = 0;
    numPosts = 0;
    for (int i = 0; i < MAX_USERS; i++) {
        for (int j = 0; j < MAX_USERS; j++) {
            following[i][j] = false;
        }
    }
}

bool Network::addUser(string usrn, string dspn) {
    if (numUsers < MAX_USERS && findID(usrn) == -1) {
        // Check if the username contains only alphanumeric characters
        for (char c : usrn) {
            if (!isalnum(c)) {
                return false;
            }
        }
        // Add the new user profile if conditions are met
        profiles[numUsers] = Profile(usrn, dspn);
        numUsers++;
        return true;
    }
    return false;
}

int main() {
  Network nw;
  // add three users
  nw.addUser("mario", "Mario");
  nw.addUser("luigi", "Luigi");
  nw.addUser("yoshi", "Yoshi");
   
  nw.follow("mario", "luigi");
  nw.follow("luigi", "mario");
  nw.follow("luigi", "yoshi");
  nw.follow("yoshi", "mario");

  // write some posts
  nw.writePost("mario", "It's a-me, Mario!");
  nw.writePost("luigi", "Hey hey!");
  nw.writePost("mario", "Hi Luigi!");
  nw.writePost("yoshi", "Test 1");
  nw.writePost("yoshi", "Test 2");
  nw.writePost("luigi", "I just hope this crazy plan of yours works!");
  nw.writePost("mario", "My crazy plans always work!");
  nw.writePost("yoshi", "Test 3");
  nw.writePost("yoshi", "Test 4");
  nw.writePost("yoshi", "Test 5");

  cout << endl;
  cout << "======= Mario's timeline =======" << endl;
  nw.printTimeline("mario");
  cout << endl;

  cout << "======= Yoshi's timeline =======" << endl;
  nw.printTimeline("yoshi");
  cout << endl;
}