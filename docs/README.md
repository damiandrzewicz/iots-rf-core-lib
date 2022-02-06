# Hello VuePress

## EEPROM Configuration

Every node can save its data to `EEPROM` memory. It is necessary due to keep data when power is down and for "cacheing" objects. "Cacheing" means when data are saved in unvolatile 'EEPROM' memory it's not necessary to ask Gateway for data. They can be read from "memory". Base abstract class is `EEPROMConfig` which is a template class.

@startuml
class EEPROMConfig<T> {
  +EEPROMConfig(int address)

  +void save()
  +void read()
  +void clear()

  +{abstract} bool isEmpty()
  +{abstract} void setEmpty()
  +{abstract} void setDefaults()

  +T &data()
  +size_t dataSize()

  -T data_
  -int address_;
}
@enduml

Template has 3 methods for saving, reading and clearing data directly in `EEPROM` memory. It contains three additional ethods for checking if structure of type `T` is empty, setting it empty and setting defaults. Constructor argument is starting memory address of initialized structure.

:::warning
Remember to do not allocate another config class with address which conflicts previously allocated `EEPROM` memory. For example if first class is allocated at address 0 and has size 12 bytes and second config is allocated at address 8, then it will due to undefined behaviour. That's a reason why method `dataSize` is introduced.
:::

:::warning
Structure cannot has `virtual` methods, because this kind of objects cannot be restored from `EEPROM` memory.
:::


### RadioConfig EEPROM implementation

@startuml

class EEPROMConfig<RadioConfigData>

class RadioConfig{
  bool isEmpty()
  void setEmpty()
  void setDefaults()
}

EEPROMConfig <|-- RadioConfig
@enduml


