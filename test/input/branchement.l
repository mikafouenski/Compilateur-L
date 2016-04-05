main ()
entier $a, entier $b; {
    $a = 0;
    $b = 1;

    si $b | lire() alors {
        ecrire(1);
    }

    si $a & lire() alors {
        ecrire(1);
    }
}
