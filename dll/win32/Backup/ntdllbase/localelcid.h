typedef struct _locale{
	LPCWSTR description;
	LPCWSTR code;
	LCID lcidHex;
	LCID lcidDec;	
}locale;

locale LocaleList[3];

void initTable();
/*
Afrikaans	af	af	1078	436	1252
Albanian	sq	sq	1052		1250
Amharic	am	am	1118		
Arabic - Algeria	ar	ar-dz	5121	1401	1256
Arabic - Bahrain	ar	ar-bh	15361		1256
Arabic - Egypt	ar	ar-eg	3073		1256
Arabic - Iraq	ar	ar-iq	2049	801	1256
Arabic - Jordan	ar	ar-jo	11265		1256
Arabic - Kuwait	ar	ar-kw	13313	3401	1256
Arabic - Lebanon	ar	ar-lb	12289	3001	1256
Arabic - Libya	ar	ar-ly	4097	1001	1256
Arabic - Morocco	ar	ar-ma	6145	1801	1256
Arabic - Oman	ar	ar-om	8193	2001	1256
Arabic - Qatar	ar	ar-qa	16385	4001	1256
Arabic - Saudi Arabia	ar	ar-sa	1025	401	1256
Arabic - Syria	ar	ar-sy	10241	2801	1256
Arabic - Tunisia	ar	ar-tn	7169		1256
Arabic - United Arab Emirates	ar	ar-ae	14337	3801	1256
Arabic - Yemen	ar	ar-ye	9217	2401	1256
Armenian	hy	hy	1067		
Assamese	as	as	1101		
Azeri - Cyrillic	az	az-az	2092		1251
Azeri - Latin	az	az-az	1068		1254
Basque	eu	eu	1069		1252
Belarusian	be	be	1059	423	1251
Bengali - Bangladesh	bn	bn	2117	845	
Bengali - India	bn	bn	1093	445	
Bosnian	bs	bs	5146		
Bulgarian	bg	bg	1026	402	1251
Burmese	my	my	1109	455	
Catalan	ca	ca	1027	403	1252
Chinese - China	zh	zh-cn	2052	804	
Chinese - Hong Kong SAR	zh	zh-hk	3076		
Chinese - Macau SAR	zh	zh-mo	5124	1404	
Chinese - Singapore	zh	zh-sg	4100	1004	
Chinese - Taiwan	zh	zh-tw	1028	404	
Croatian	hr	hr	1050		1250
Czech	cs	cs	1029	405	1250
Danish	da	da	1030	406	1252
Divehi; Dhivehi; Maldivian	dv	dv	1125	465	
Dutch - Belgium	nl	nl-be	2067	813	1252
Dutch - Netherlands	nl	nl-nl	1043	413	1252
Edo			1126	466	
English - Australia	en	en-au	3081		1252
English - Belize	en	en-bz	10249	2809	1252
English - Canada	en	en-ca	4105	1009	1252
English - Caribbean	en	en-cb	9225	2409	1252
English - Great Britain	en	en-gb	2057	809	1252
English - India	en	en-in	16393	4009	
English - Ireland	en	en-ie	6153	1809	1252
English - Jamaica	en	en-jm	8201	2009	1252
English - New Zealand	en	en-nz	5129	1409	1252
English - Phillippines	en	en-ph	13321	3409	1252
English - Southern Africa	en	en-za	7177		1252
English - Trinidad	en	en-tt	11273		1252
English - United States	en	en-us	1033	409	1252
English - Zimbabwe	en		12297	3009	1252
Estonian	et	et	1061	425	1257
Faroese	fo	fo	1080	438	1252
Farsi - Persian	fa	fa	1065	429	1256
Filipino			1124	464	
Finnish	fi	fi	1035		1252
French - Belgium	fr	fr-be	2060		1252
French - Cameroon	fr		11276		
French - Canada	fr	fr-ca	3084		1252
French - Congo	fr		9228		
French - Cote d'Ivoire	fr		12300		
French - France	fr	fr-fr	1036		1252
French - Luxembourg	fr	fr-lu	5132		1252
French - Mali	fr		13324		
French - Monaco	fr		6156		1252
French - Morocco	fr		14348		
French - Senegal	fr		10252		
French - Switzerland	fr	fr-ch	4108		1252
French - West Indies	fr		7180		
Frisian - Netherlands			1122	462	
FYRO Macedonia	mk	mk	1071		1251
Gaelic - Ireland	gd	gd-ie	2108		
Gaelic - Scotland	gd	gd	1084		
Galician	gl		1110	456	1252
Georgian	ka		1079	437	
German - Austria	de	de-at	3079		1252
German - Germany	de	de-de	1031	407	1252
German - Liechtenstein	de	de-li	5127	1407	1252
German - Luxembourg	de	de-lu	4103	1007	1252
German - Switzerland	de	de-ch	2055	807	1252
Greek	el	el	1032	408	1253
Guarani - Paraguay	gn	gn	1140	474	
Gujarati	gu	gu	1095	447	
Hebrew	he	he	1037		1255
HID (Human Interface Device)			1279		
Hindi	hi	hi	1081	439	
Hungarian	hu	hu	1038		1250
Icelandic	is	is	1039		1252
Igbo - Nigeria			1136	470	
Indonesian	id	id	1057	421	1252
Italian - Italy	it	it-it	1040	410	1252
Italian - Switzerland	it	it-ch	2064	810	1252
Japanese	ja	ja	1041	411	
Kannada	kn	kn	1099		
Kashmiri	ks	ks	1120	460	
Kazakh	kk	kk	1087		1251
Khmer	km	km	1107	453	
Konkani			1111	457	
Korean	ko	ko	1042	412	
Kyrgyz - Cyrillic			1088	440	1251
Lao	lo	lo	1108	454	
Latin	la	la	1142	476	
Latvian	lv	lv	1062	426	1257
Lithuanian	lt	lt	1063	427	1257
Malay - Brunei	ms	ms-bn	2110		1252
Malay - Malaysia	ms	ms-my	1086		1252
Malayalam	ml	ml	1100		
Maltese	mt	mt	1082		
Manipuri			1112	458	
Maori	mi	mi	1153	481	
Marathi	mr	mr	1102		
Mongolian	mn	mn	2128	850	
Mongolian	mn	mn	1104	450	1251
Nepali	ne	ne	1121	461	
Norwegian - Bokml	nb	no-no	1044	414	1252
Norwegian - Nynorsk	nn	no-no	2068	814	1252
Oriya	or	or	1096	448	
Polish	pl	pl	1045	415	1250
Portuguese - Brazil	pt	pt-br	1046	416	1252
Portuguese - Portugal	pt	pt-pt	2070	816	1252
Punjabi	pa	pa	1094	446	
Raeto-Romance	rm	rm	1047	417	
Romanian - Moldova	ro	ro-mo	2072	818	
Romanian - Romania	ro	ro	1048	418	1250
Russian	ru	ru	1049	419	1251
Russian - Moldova	ru	ru-mo	2073	819	
Sami Lappish			1083		
Sanskrit	sa	sa	1103		
Serbian - Cyrillic	sr	sr-sp	3098		1251
Serbian - Latin	sr	sr-sp	2074		1250
Sesotho (Sutu)			1072	430	
Setsuana	tn	tn	1074	432	
Sindhi	sd	sd	1113	459	
Sinhala; Sinhalese	si	si	1115		
Slovak	sk	sk	1051		1250
Slovenian	sl	sl	1060	424	1250
Somali	so	so	1143	477	
Sorbian	sb	sb	1070		
Spanish - Argentina	es	es-ar	11274		1252
Spanish - Bolivia	es	es-bo	16394		1252
Spanish - Chile	es	es-cl	13322		1252
Spanish - Colombia	es	es-co	9226		1252
Spanish - Costa Rica	es	es-cr	5130		1252
Spanish - Dominican Republic	es	es-do	7178		1252
Spanish - Ecuador	es	es-ec	12298		1252
Spanish - El Salvador	es	es-sv	17418		1252
Spanish - Guatemala	es	es-gt	4106		1252
Spanish - Honduras	es	es-hn	18442		1252
Spanish - Mexico	es	es-mx	2058		1252
Spanish - Nicaragua	es	es-ni	19466		1252
Spanish - Panama	es	es-pa	6154		1252
Spanish - Paraguay	es	es-py	15370		1252
Spanish - Peru	es	es-pe	10250		1252
Spanish - Puerto Rico	es	es-pr	20490		1252
Spanish - Spain (Traditional)	es	es-es	1034		1252
Spanish - Uruguay	es	es-uy	14346		1252
Spanish - Venezuela	es	es-ve	8202		1252
Swahili	sw	sw	1089	441	1252
Swedish - Finland	sv	sv-fi	2077		1252
Swedish - Sweden	sv	sv-se	1053		1252
Syriac			1114		
Tajik	tg	tg	1064	428	
Tamil	ta	ta	1097	449	
Tatar	tt	tt	1092	444	1251
Telugu	te	te	1098		
Thai	th	th	1054		
Tibetan	bo	bo	1105	451	
Tsonga	ts	ts	1073	431	
Turkish	tr	tr	1055		1254
Turkmen	tk	tk	1090	442	
Ukrainian	uk	uk	1058	422	1251
Unicode		UTF-8	0		
Urdu	ur	ur	1056	420	1256
Uzbek - Cyrillic	uz	uz-uz	2115	843	1251
Uzbek - Latin	uz	uz-uz	1091	443	1254
Venda			1075	433	
Vietnamese	vi	vi	1066		1258
Welsh	cy	cy	1106	452	
Xhosa	xh	xh	1076	434	
Yiddish	yi	yi	1085		
Zulu	zu	zu	1077	435	
*/