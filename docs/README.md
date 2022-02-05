# Hello VuePress

## EEPROM Configuration

Node has possibility to save configuration in `EEPROM` memory. Interface class, which can be implemented for different kinds of configurations is following:

@startuml
interface EEPROMConfig{
  {abstract} void save()
  {abstract} void read()
  {abstract} void clear()
}

class Data{
  {abstract} bool isEmpty()
  {abstract} bool setEmpty()
}

Data -left-* EEPROMConfig
@enduml

Every single method is abstract and body should be implemented in derived class. Class `EEPROMConfig` contains `Data` class with is treated as a model and should be extended in derived class too.

### RadioConfig EEPROM implementation

@startuml
interface EEPROMConfig
class RadioConfig{
  void save()
  void read()
  void clear()
}

class Data{

}

EEPROMConfig <|-- RadioConfig
@enduml


