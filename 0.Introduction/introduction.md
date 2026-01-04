## Прошивка CH32V003 через программатор WCH-LinkE

### 1. Подготовка системы: права доступа и правила udev
Без этого программатор и плата не будут доступны для обычного пользователя.

#### 1.1 Настройка группы `plugdev`
Сначала проверьте, существует ли группа `plugdev` и есть ли в ней ваш пользователь:
```bash
grep plugdev /etc/group
```
Если группы нет, создайте её:
```bash
sudo groupadd plugdev
```
Если вашего имени пользователя (например, `slava`) нет в строке вывода, добавьте себя в группу:
```bash
sudo usermod -a -G plugdev $USER
```
**Важно**: Для применения изменений необходимо выйти из системы и зайти заново.

#### 1.2 Установка правил udev
Эти правила автоматически дают права на доступ к USB-устройствам WCH (программатору и платам) при их подключении.
1.  Скачайте архив с тулчейном (`MRS_Toolchain_Linux_x64_V230.tar.xz`) или полной IDE с [официального сайта](https://www.mounriver.com/download).
2.  Распакуйте или найдите внутри файл `50-wch.rules` (обычно в папке `beforeinstall`). Если этого файла не окажется, то скопируйте текст ниже:
```
ATTR{idVendor}=="1a86", ATTR{idProduct}=="8010", MODE="0660", GROUP="plugdev"
ATTR{idVendor}=="1a86", ATTR{idProduct}=="8012", MODE="0660", GROUP="plugdev"
ATTR{idVendor}=="4348", ATTR{idProduct}=="55e0", MODE="0660", GROUP="plugdev"
```
3.  Скопируйте его в системную директорию и обновите конфигурацию:
    ```bash
    sudo cp путь/до/50-wch.rules /etc/udev/rules.d/
    sudo udevadm control --reload
    ```
**Почему это важно**: Без этих правил программатор может "заблокироваться" при обновлении прошивки, а доступ к платам будет требовать прав root.

### 2. Установка инструментов
**Установка `wlink`** (утилита для работы с программатором):
```bash
git clone https://github.com/ch32-rs/wlink.git
cd wlink-git
makepkg -si
```

### 3. Подключение CH32V003  к программатору
Для прошивки CH32V003 подключите WCH-LinkE к микроконтроллеру следующим образом:
- **SWDIO/TMS** программатора -> к **SWD** на CH32V003.
- **GND** программатора -> к **G** на CH32V003.
- **3.3V** программатора -> к **V** на CH32V003, если плата не питается самостоятельно.

### 4. Использование wlink для прошивки

#### 4.1 Проверка программатора
```bash
wlink status
```
Результат:
``` bash
wlink status
12:42:12 [INFO] Connected to WCH-Link v2.18(v38) (WCH-LinkE-CH32V305)
12:42:12 [INFO] Attached chip: CH32V003 [CH32V003F4P6] (ChipID: 0x00300510)
12:42:12 [INFO] Chip ESIG: FlashSize(16KB) UID(cd-ab-06-3f-d2-bd-a5-a8)
12:42:12 [INFO] Flash protected: false
12:42:12 [INFO] RISC-V ISA(misa): Some("RV32CEX")
12:42:12 [INFO] RISC-V arch(marchid): Some("WCH-V2A")
12:42:12 [WARN] The halt status may be incorrect because detaching might resume the MCU
12:42:12 [INFO] Dmstatus {
    # ...
}
12:42:12 [INFO] Dmcontrol {
    # ...
}
12:42:12 [INFO] Hartinfo {
    # ...
}
12:42:12 [INFO] Abstractcs {
    # ...
}
12:42:12 [INFO] haltsum0: 0x1
```

#### 4.2 Компиляция и загрузка прошивки
1.  Скомпилируйте код в бинарный файл:
    ```bash
    riscv64-elf-gcc -march=rv32ec -mabi=ilp32e -nostdlib -T link.ld -o firmware.elf main.c
    riscv64-elf-objcopy -O binary firmware.elf firmware.bin
    ```
2.  Загрузите прошивку в микроконтроллер:
    ```bash
    wlink flash firmware.bin --address 0x0
    ```

### 5. Использование OpenOCD для прошивки
OpenOCD используйте из скаченного архива с тулчейном (`MRS_Toolchain_Linux_x64_V230.tar.xz`) с [официального сайта](https://www.mounriver.com/download).

#### 5.1 Компиляция и загрузка прошивки
1.  Скомпилируйте код в бинарный файл:
    ```bash
    riscv64-elf-gcc -march=rv32ec -mabi=ilp32e -nostdlib -T link.ld -o firmware.elf main.c
    riscv64-elf-objcopy -O binary firmware.elf firmware.bin
    ```
2.  Загрузите прошивку в микроконтроллер:
    ```bash
    ./openocd -f ./wch-risv.cfg -c "programm firmware.elf verify reset exit"
    ```
    Так как openocd и конфигурационный файл находятся в одной директории. В дальнешйнем можете создать ссылку `openocd-wch` на openocd, чтобы не использовать полный путь.

## Troubleshooting

- **Проблема**: `Error: WCH-Link is connected, but is not in RV mode`.
  - **Решение**: Выполните команду `wlink mode-switch --rv`.

- **Проблема**: `rustc: error while loading shared libraries: libLLVM.so.21.1: cannot open shared object file...`
  - **Решение**: Эта ошибка возникает при сборке `wlink`. Установите `rustup` и используйте стабильную версию Rust:
    ```bash
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
    source $HOME/.cargo/env
    cd wlink
    rustup override set stable
    makepkg -si
    ```

- **Проблема**: Программатор не обнаруживается (`wlink status` не работает).
  - **Решение**: Убедитесь, что:
    1.  Файл с правилами `/etc/udev/rules.d/50-wch.rules` существует.
    2.  Ваш пользователь входит в группу `plugdev` (команда `groups`).