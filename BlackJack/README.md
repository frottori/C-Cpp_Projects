# BlackJack 

Να γραφεί ένα πρόγραμμα το οποίο να υλοποιεί μία απλοποιημένη έκδοση του παιχνιδιού «black-jack» σύμφωνα με τους παρακάτω κανόνες:  

- Αρχικά, θα πρέπει να γίνεται τράβηγμα ενός φύλλου για τη «μάνα» και δύο φύλλων για τον παίκτη. Αν τα δύο φύλλα του παίκτη είναι «Άσσος» και οποιαδήποτε φιγούρα ή 10, ο παίκτης  κερδίζει  και  το  πρόγραμμα  τερματίζει.  Αν  είναι  ίδια  (π.χ.  δύο  τεσσάρια),  το πρόγραμμα να ρωτάει τον παίκτη αν θέλει να τα διαχωρίσει σε δύο ξεχωριστά παιξίματα («χέρια»). Αν απαντήσει ναι, τότε το κάθε φύλλο παίζεται ξεχωριστά σαν να έπαιζαν δύο διαφορετικοί παίκτες. Για κάθε παίξιμο φύλλου, το πρόγραμμα να ρωτάει τον παίκτη αν επιθυμεί να τραβήξει κι άλλο φύλλο μέχρι αυτός να του πει ότι θέλει να σταματήσει. Αν το άθροισμα των φύλλων που έχει τραβήξει ο παίκτης υπερβεί το 21, τότε ο παίχτης χάνει («κάηκε»). 

- Ο παίκτης επιτρέπεται να σταματήσει ή να τραβήξει φύλλο χωρίς κάποιο περιορισμό. Η μάνα είναι υποχρεωμένη να τραβάει φύλλα μέχρι το συνολικό άθροισμα των φύλλων της να υπερβεί το 16, ακόμα και αν σε κάποια φάση έχει μεγαλύτερο άθροισμα από τον παίκτη. Αν υπερβεί το 16 (και όχι το 21), η μάνα είναι υποχρεωμένη να σταματήσει, ακόμα και αν ο παίκτης έχει μεγαλύτερο άθροισμα. Όπως και με τον παίκτη, αν η μάνα υπερβεί το 21 χάνει. 

- Αν δεν «καεί» κανένας από τους δύο, κερδίζει αυτός που έχει το μεγαλύτερο συνολικό άθροισμα φύλλων. Αν έχουν το ίδιο, τότε το παιχνίδι λήγει ισόπαλο.  

Για το τράβηγμα του φύλλου, να χρησιμοποιήσετε την rand() η οποία να επιστρέφει την τιμή του φύλλου. Οι πιθανές τιμές φύλλου (θετικός ακέραιος που ανήκει στο [1, 13]) : 
- «Άσσος»: 1, «Δύο»: 2, …, «Δέκα»: 10
- «Βαλές»: 11
- «Ντάμα»: 12
- «Ρήγας»: 13
Οι «φιγούρες» (Βαλές, Ντάμα και Ρήγας) μετράνε για 10, ενώ για απλότητα θεωρήστε ότι ο «Άσσος» μετράει μόνο για 1 (κανονικά είναι για 1 ή 11). Το πρόγραμμα να εμφανίζει την τιμή  των  φύλλων.  Θεωρήστε  επίσης  ότι  το  παιχνίδι  παίζεται  με  πολλές  τράπουλες. Συμβουλευτείτε τα παρακάτω παραδείγματα εξόδου, ώστε να δείτε τα μηνύματα που πρέπει να εμφανίζει το πρόγραμμά σας: 

### Παράδειγμα\_1: 

First card of dealer is 7 First card of player is 1 Second card of player is 'J' Player gets 21. Player wins! 

### Παράδειγμα\_2: 

First card of dealer is 2 

First card of player is 9 

Second card of player is 8 

Hand\_1: Do you want to draw another card (1:for yes)? 1 Player draw 'K' 

Player has total 27 

Player lost! 

### Παράδειγμα\_3: 

First card of dealer is 10 

First card of player is 'Q' 

Second card of player is 9 

Hand\_1: Do you want to draw another card (1:for yes)? 0 Dealer draw 9 

Dealer has total 19 

Hand\_1: Nobody wins!

### Παράδειγμα\_4: 

First card of dealer is 7 

First card of player is 3 

Second card of player is 1 

Hand\_1: Do you want to draw another card (1:for yes)? 1 Player draw 7 

Player has total 11 

Hand\_1: Do you want to draw another card (1:for yes)? 1 Player draw 'Q' 

Player has total 21 

Dealer draw 'K' 

Dealer has total 17 

Hand\_1: Player wins! 

### Παράδειγμα\_5: 

First card of dealer is 5 

First card of player is 'J' 

Second card of player is 4 

Hand\_1: Do you want to draw another card (1:for yes)? 0 Dealer draw 10 

Dealer has total 15 

Dealer draw 'K' 

Dealer has total 25 

Player wins! 

### Παράδειγμα\_6: 

First card of dealer is 8 

First card of player is 'J' 

Second card of player is 'J' 

Split the cards (1:for yes)? 1 

Hand\_1: Do you want to draw another card (1:for yes)? 1 Player draw 'K' 

Player has total 20 

Hand\_1: Do you want to draw another card (1:for yes)? 0 Hand\_2: Do you want to draw another card (1:for yes)? 1 Player draw 9 

Player has total 19 

Hand\_2: Do you want to draw another card (1:for yes)? 0 Dealer draw 7 

Dealer has total 15 

Dealer draw 2 

Dealer has total 17 

Hand\_1: Player wins! 

Hand\_2: Player wins! 
