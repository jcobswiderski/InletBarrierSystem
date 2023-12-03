Gotowy plik uruchomieniowy aplikacji znajduje się w folderze głównym pod nazwą IbfControlPanel.exe. Wystarczy go przenieść na pulpit komputera IOS oraz uruchomić. 

Aby aplikacja działała poprawnie musi być podłączony moduł arduino 2560 (z zainstalowanymi sterownikami) przez USB oraz ustawiony port modułu COM30. 

W katalogu arduino-1.8.9 dostępne są sterowniki Arduino 2560 działające na systemie Windows XP. Poprawnie zainstalowane sterowniki w menadżerze urządzeń wyświetlają urządzenie o nazwie Arduino 2560. Podczas instalacji sterowników system wyświetla komunikat, że sterowniki nie mogą zostać sprawdzone, należy to zignorować.

Archiwum CH341SER.ZIP zawiera aplikację pozwalającą na zainstalowanie sterowników dla Arduino Nano (ATmega328P Old Bootloader) - w przypadku chęci korzystania z modułu w celu podświetlenia panelu IBF, należy je zainstalować.

Plik ibf-arduino.ino w katalogu ibf-arduino zawiera pełną konfigurację dla mikrokontrolera Arduino Mega 2560 w tym działanie poszczególnych diod, przycisków, przełączników oraz obsługę sygnałów sterujących. Komunikacja między aplikacją .NET a mikrokontrolerem bazuje na UART. Program korzysta z zewnętrznej biblioteki Arduino ezButton 1.0.4 do obsługi działania przełączników.

Katalog IbfControlPanel_dotnet_30-11-2023 zawiera pełny projekt aplikacji do zdalnej obsłgi panelu IBF. Aplikacja napisana została na system Windows XP, wersja .NET Framework 3.5; W aplikacji został statycznie określony port odbiornika - arduino. Ustawiony został na port COM30. Identyczny port musi zostać ustawiony na komputerze IOS dla Arduino Mega.

Istnieje możliwość rozbudowania programu o obsługę drugiego mikrokontrolera (np. Arduino Nano) w celu obsługi podświetlenia panelu. Należy dodać drugi port i uwzględnić go dla konkretnych syganłów sterujących w funkcjach programu.

Plik dotnetfx35.exe jest instalatorem .NET w wersji 3.5. Działa na systemie Windows XP. Należy z niego skorzystać w przypadku komunikatu o braku odpowiedniej wersji środowiska .NET.

Katalog ibf_pcb_project zawiera pliki programu Kicad. W projekcie stworzono prototyp płytki PCB do panelu IBF.