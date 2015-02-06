# Cayley-HHVM
A [Cayley][Cayley-Repo] driver for [HHVM][HHVM-Repo]

## Usage
```hack
$Cayley = new Cayley("127.0.0.1:64210");
// Adding a record
$Cayley->write('I','speak','English');
$Cayley->write('I','speak','English','I am a label');
// Deleting a record
$Cayley->delete('I','speak','English');
$Cayley->delete('I','speak','English','I am a label');
// Querying the database
$g = $Cayley->g();
$Result = $g->V("I")->Out("speak")->All();
print_r($Result);
```

[Cayley-Repo]:https://github.com/google/cayley
[HHVM-Repo]:https://github.com/facebook/hhvm
