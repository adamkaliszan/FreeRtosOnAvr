Wgranie oprogramowania do mikrokontrolera AtMega16

Płytka ma 2 błędy :(. Należy je przed uruchomieniem progamatora JTAG skorygować. Na górnej stronie płytki do masy (najlepiej w prawym górnym rogu, gdzie jest przycisk reset) nalezy dolutować opornik 10 kilo omów. Drugi koniec opornika należy podłączyć (za pośrednictwem kynaru) do nóżki nr 12 (PD3).

<p align=center><img border="0" src="http://makgywer.vipserv.org/wyklady/LabSSW/hardware/prog_patch_gora.gif"></p>
Drugi błąd należy skorygować rozcinając jedną ścieżkę na dolnej stronie płytki. NAstępnie nalezy kynarem podlutowac tą ścieżkę do innego wyjścia na złączu IDC10.
<p align=center><img border="0" src="http://makgywer.vipserv.org/wyklady/LabSSW/hardware/prog_patch_dol.gif"></p>
<p>Po skorygowniu błędów można przejść do uruchamiania programatora. Do wgrywania oprogramowania na procesor AtMega16 (ten co obsługuje programator JTAG ICE) złuży złącze J3. Jako programator można wykorzystać programator Stk500, który znajduje się na uruchamianej płytce. W tym celu należy połączyć ze sobą na wprowst złącza J2 i j3. Następnie należy przestawić zworki FT_RX i FT_TX na pozycję skrajnie lewe. Najpierw należy skonfigurować bezpieczniki, najlepiej za pomocą AVR Studio:
<ul>
 <li>zewnętrzny kwarc,</li>
 <li>włączony bootloader,</li>
 <li>rozmiar kodu bootloadera: 1 kB.</li>
</ul>
Następnie należy wgrać kod bootloadera. Jest on dostępny <a href="http://makgywer.vipserv.org/wyklady/LabSSW/hardware/boot.hex">tutaj</a>.
</p>
<p>Po wgraniu kodu bootloadera, przestawiamy zworki na pozycje skrajnie prawe. W AVR studio uruchamiany AVRProg i wybieramy obraz programatora JTAG do wgrania. Po wgraniu obrazu (za pomocą bootloadera) pojawi się informacja o błędzie. Należy ją zignorować, a następnie wyłączyć bootloader (wykorzystując programator STK500, należy pamiętać o przestawieniu zworek). Od tej pory mamy działający programator JTAG ICE. Dokłądniejszy opis jest zawarty na stronie: <a href="http://www.scienceprog.com/build-your-own-avr-jtagice-clone/" target=_blank>http://www.scienceprog.com/build-your-own-avr-jtagice-clone</a>.
</p>
