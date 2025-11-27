**DND Password Generator**
DND Password Generator is a Linux password generator that uses the main dice from the Dungeons & Dragons game. The generated password is 13 characters long and combines letters, numbers, and special symbols in a random order, inspired by DND dice rolls.

Here’s how it works:

- D20 + D12 + D6: generates 7 random letters (uppercase or lowercase).
- D10: generates 3 random numbers.
- D8: generates 2 special symbols (ASCII symbols from 33 to 40).
- D4: determines the random order of combining the above elements and adds an extra special symbol “@”.

The program uses /dev/urandom to increase randomness, shuffles all the elements according to a random order, and constructs the final password.

The result is a secure, unpredictable password inspired by DND dice mechanics.
