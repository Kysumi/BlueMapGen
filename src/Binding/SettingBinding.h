//
// Created by Scott on 12/07/2020.
//

#ifndef ONGRID_SETTINGBINDING_H
#define ONGRID_SETTINGBINDING_H


class SettingBinding {

public:
    static void bind();

private:
    static void *
    GetRandomNumber(void *callee, bool isConstructCall, void **arguments, unsigned short argumentCount, void *callbackState);
};


#endif //ONGRID_SETTINGBINDING_H
