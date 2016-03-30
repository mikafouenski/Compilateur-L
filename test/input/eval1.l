add (entier $a, entier $b)
{
 retour $a + $b;
}

mult (entier $a, entier $b)
{
 retour $a * $b;
}

main()
entier $a, entier $b, entier $op;
{
 $a = lire();
 $b = lire();
 $op = lire();
 si $op = 1 alors {
  ecrire(add($a, $b));
 }
 sinon {
  si $op = 2 alors {
   ecrire(mult($a, $b));
  }
 }
}
