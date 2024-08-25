# PACMAN für AC1-CP/M
Das PACMAN-Spiel wurde von Dave Nixon im Juli 1981 in C geschrieben.
Im Jahr 1982 ist es von Jeff Martin auf den BDS C-Compiler
portiert worden.
Da die Terminalausgabe zeitkritisch ist, wurden die C-internen E/A-Funktionen
nicht verwendet. Stattdessen erfolgte die Zeichenausgabe direkt auf die
(damals verwendete) serielle Terminal-Schnittstelle.

Das hier vorliegende PACMAN-Spiel basiert auf diesem Quellcode und wurde
an den AC1 mit einem 64x32-Bildschirm angepasst.
Außerdem wird bei Vorhandensein eines Color-BWS die Farbausgabe unterstützt.
Es benötigt ein lauffähiges CP/M-System, welches eine Escape-Sequenz zur
Kursorpositionierung sowie die Kommandos Kursor_ON und Kursor_OFF unterstützt
(z. B. [HRCPM12](https://github.com/friedertonn/HRCPM12), Anpassungen sind in 
der Datei PACDEFS.H vorzunehmen).
Das CP/M-System sollte mit Tastatur-Pufferspeicher und Interrupt arbeiten,
damit das Zusammenspiel zwischen den BIOS-Aufrufen CONST (console status)
und CONIN (console input) optimal funktioniert.
Damit PACMAN auch bei 2 MHz CPU-Takt flüssig läuft, erfolgen alle 
Ein-/Ausgaberoutinen über BIOS-Aufrufe. 

Zum Kompilieren wird der BDS C-Compiler V1.6 benötigt.
Zunächst müssen die C-Dateien kompiliert werden:
```
CC PACMAN.C
CC PACMONST.C
CC PACUTIL.C
```

Anschließend werden sie zu einer ausführbaren Datei PACMAN.COM gelinkt:
```
CLINK PACMAN PACMONST PACUTIL DEFF
```

Die nachfolgende Abbildung zeigt PACMAN im JKCEMU:

![PACMAN im JKCEMU](https://github.com/friedertonn/PACMAN/blob/main/Fotos/PACMAN.png?raw=true)

Es gibt vier verschiedene Spiel-Typen:

|            | color-BWS | b/w-BWS |                                |
|------------|:---------:|:-------:|--------------------------------|
| **Start:** | 1         | 5       | normales Spiel                 |
|            | 2         | 6       | blinkende Monster              |
|            | 3         | 7       | intelligente Monster           |
|            | 4         | 8       | blinkende intelligente Monster |

Die Steuerung erfolgt mit den Kursortasten (AC1- und CP/M-Tastenbelegung funktionieren beide).
Mit der Leertaste kann das Spiel angehalten werden. Q oder Ctrl-C beenden das Spiel. 
