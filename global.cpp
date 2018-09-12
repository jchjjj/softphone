#include "global.h"
#include <model/config.h>

Global * Global::global = NULL;

Global::Global()
{
    //global = this;
    mainwindow = nullptr;
    manager = nullptr;

    config = new Config;
}
