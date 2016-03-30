entier $i, entier $j, entier $k, entier $somme;
main() {
  $somme = 0;
  pour $i = 0; $i < 10; $i = $i + 1; faire {
    pour $j = 0; $j < $i; $j = $j + 1; faire {
      pour $k = 0; $k < $j; $k = $k + 1; faire {    
        $somme = $somme + $i + $j + $k;
      }
    }
  }
  ecrire ( $somme );
}
