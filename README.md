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
- **4** layers board: ```SIG -  GND - PWR - SIG```; ✔
- Manufacturer: *JLCPCB*. ✔

### Power Line:
- [x] Definir Bateria; ✔
    - Li-ion 14500 / 2S / 7.4 V / 800 mAh

- [x] Definir Como Será o carregamento da bateria; ✔
    - *Tirar* a bateria e usar o **imax B6**
    - Conectores: *XT90*(```Male```) + *BMS*([Exemplos](https://pt.aliexpress.com/item/1005006041486774.html?spm=a2g0o.productlist.main.1.246b67a4PkHivT&algo_pvid=ff23d1ad-6cca-46d0-84ef-1046141dedfa&algo_exp_id=ff23d1ad-6cca-46d0-84ef-1046141dedfa-0&pdp_ext_f=%7B"order"%3A"502"%2C"eval"%3A"1"%7D&pdp_npi=4%40dis%21BRL%216.29%216.29%21%21%217.42%217.42%21%402103146f17418148316958806e4362%2112000035452245642%21sea%21BR%214067292657%21X&curPageLogUid=rQEBQMHSnDL5&utparam-url=scene%3Asearch%7Cquery_from%3A)) na PCB & Conector *XH* para balanceamento 

- [ ] Proteção ESD, polaridade reversa e fusível;
- [x] Regulador de tensão ```8.4 -> 5``` V (```Mini DC-DC```) + *Decoupling*; ✔
- [x] LDO ```5 -> 3.3``` V (```AMS1117```) + *Decoupling*. ✔
