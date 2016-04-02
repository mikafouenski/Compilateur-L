entier $a;
main() {
    $a = 6 * 7 / 8 + 32 - 13;
    ecrire($a);
    si $a = 24 alors {
        ecrire(1);
    } sinon {
        ecrire(0);
    }
    si $a < 25 alors {
        ecrire(1);
    } sinon {
        ecrire(0);
    }
    si 23 < $a alors {
        ecrire(1);
    } sinon {
        ecrire(0);
    }
}
