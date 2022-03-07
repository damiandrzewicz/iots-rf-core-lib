#pragma once

struct UUIDConfigData{
    byte uuid[16];
};

// class UUIDConfig : public EEPROMConfig<UUIDConfigData>
// {
// public:
//     UUIDConfig(int address) : EEPROMConfig(address) {}

//     virtual bool isEmpty() override{
//         return data_.uuid[0] == emptyVal_;
//     }

//     virtual void setEmpty() override {
//         data_.uuid[0] = emptyVal_;
//     }

//     virtual void setDefaults() override {
//         memset(data_.uuid, 0, sizeof(data_.uuid));
//     }
// };