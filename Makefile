# Caminho para ESP-IDF
IDF_PATH ?= $(HOME)/esp/v5.4.1/esp-idf

# Caminho para o script idf.py
IDF_PY ?= $(IDF_PATH)/tools/idf.py

IDF := idf.py

# Caminho para o Python (opcional)
PYTHON ?= python3

# Alvo padrão
all: build

.PHONY: build flash monitor clean menuconfig size erase

#all:
#	bash -c "source $(IDF_PATH)/export.sh && idf.py build encrypted-flash monitor"

build:
	bash -c "source $(IDF_PATH)/export.sh && idf.py build"

flash:
	bash -c "source $(IDF_PATH)/export.sh && idf.py flash"

monitor:
	bash -c "source $(IDF_PATH)/export.sh && idf.py monitor"

program:
	bash -c "source $(IDF_PATH)/export.sh && idf.py flash monitor"

menu:
	bash -c "source $(IDF_PATH)/export.sh && idf.py menuconfig"

clean:
	bash -c "source $(IDF_PATH)/export.sh && idf.py fullclean"

size:
	bash -c "source $(IDF_PATH)/export.sh && idf.py size"

erase:
	bash -c "source $(IDF_PATH)/export.sh && idf.py erase-flash"