entier	mot_clef	entier
$tab	id_variable	$tab
[	symbole	CROCHET_OUVRANT
10	nombre	10
]	symbole	CROCHET_FERMANT
;	symbole	POINT_VIRGULE
main	id_fonction	main
(	symbole	PARENTHESE_OUVRANTE
)	symbole	PARENTHESE_FERMANTE
entier	mot_clef	entier
$a	id_variable	$a
;	symbole	POINT_VIRGULE
{	symbole	ACCOLADE_OUVRANTE
$a	id_variable	$a
=	symbole	EGAL
0	nombre	0
;	symbole	POINT_VIRGULE
tantque	mot_clef	tantque
$a	id_variable	$a
<	symbole	INFERIEUR
10	nombre	10
faire	mot_clef	faire
{	symbole	ACCOLADE_OUVRANTE
$tab	id_variable	$tab
[	symbole	CROCHET_OUVRANT
$a	id_variable	$a
]	symbole	CROCHET_FERMANT
=	symbole	EGAL
$a	id_variable	$a
;	symbole	POINT_VIRGULE
$a	id_variable	$a
=	symbole	EGAL
$a	id_variable	$a
+	symbole	PLUS
1	nombre	1
;	symbole	POINT_VIRGULE
}	symbole	ACCOLADE_FERMANTE
$a	id_variable	$a
=	symbole	EGAL
0	nombre	0
;	symbole	POINT_VIRGULE
tantque	mot_clef	tantque
$a	id_variable	$a
<	symbole	INFERIEUR
10	nombre	10
faire	mot_clef	faire
{	symbole	ACCOLADE_OUVRANTE
ecrire	mot_clef	ecrire
(	symbole	PARENTHESE_OUVRANTE
$tab	id_variable	$tab
[	symbole	CROCHET_OUVRANT
$a	id_variable	$a
]	symbole	CROCHET_FERMANT
)	symbole	PARENTHESE_FERMANTE
;	symbole	POINT_VIRGULE
$a	id_variable	$a
=	symbole	EGAL
$a	id_variable	$a
+	symbole	PLUS
1	nombre	1
;	symbole	POINT_VIRGULE
}	symbole	ACCOLADE_FERMANTE
}	symbole	ACCOLADE_FERMANTE
	symbole	FIN