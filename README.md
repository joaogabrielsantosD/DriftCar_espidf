# To Do List

## Firmware

### State Of Charge:
- [ ] Definir metodo de aquisição e filtro, **tensão em 8.5 volts**;
- [ ] Task para atualizar a tensão e porcentagem;
- [ ] Garantir que o ponteiro atualize mesmo em outra task.


### ESP-NOW
- [ ] Sincronização com o controle via ESP-NOW.


### LEDs
- [ ] Leitura do acionamento do botão para os faróis.
- [ ] Definir frequência mínima para o estado "padrão" da luz de freio.

## Hardware

### Layers:
- **4** layers board: ```SIG/PWR -  GND - SIG/PWR - GND```; ✔
- Manufacturer: *JLCPCB*. ✔

### Power Line:
- [ ] Definir Bateria;
- [ ] Definir Como Será o carregamento da bateria;
- [ ] Proteção ESD, polaridade reversa e fusível;
- [ ] Regulador de tensão ```x -> 5``` volts + *Decoupling*;
- [ ] LDO ```5 -> 3.3``` volts + Decoupling.
