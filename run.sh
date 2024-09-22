#!/bin/env bash

helpFunction() {
  echo ""
  echo "Usage: $0 <command>"
  echo -e "\tcommand: Tells the script what to do. Options:"
  echo -e "\t\tflash: Builds and flashes the board"
  echo -e "\t\tbuild: Builds the project"
  echo -e "\t\tbear:  Creates the compile_commands.json file"
  echo -e "\t\thelp:  Shows this prompt"
}

if [[ $1 == "help" || -z $1 ]]; then
    helpFunction
elif [[ $1 == "flash" ]]; then
    (mkdir -p bin)
    (arm-none-eabi-gcc src/main.c src/syscalls.c src/sysmem.c src/uart.c src/simpit.c src/ringbuffer.c src/timers.c startup_stm32l433rctxp.s -T STM32L433RCTXP_FLASH.ld -Iinclude -o bin/blink.elf -mcpu=cortex-m4 -mthumb)
    (cd bin; openocd -f interface/stlink.cfg -f target/stm32l4x.cfg -c "program blink.elf verify reset exit")
elif [[ $1 == "build" ]]; then
    (mkdir -p bin)
    (arm-none-eabi-gcc src/main.c src/syscalls.c src/sysmem.c src/uart.c src/simpit.c src/ringbuffer.c src/timers.c startup_stm32l433rctxp.s -T STM32L433RCTXP_FLASH.ld -Iinclude -o bin/blink.elf -mcpu=cortex-m4 -mthumb)
elif [[ $1 == "bear" ]]; then
    (mkdir -p bin)
    (bear -- arm-none-eabi-gcc src/main.c src/syscalls.c src/sysmem.c src/uart.c src/simpit.c src/ringbuffer.c src/timers.c startup_stm32l433rctxp.s -T STM32L433RCTXP_FLASH.ld -Iinclude -o bin/blink.elf -mcpu=cortex-m4 -mthumb)
fi
