#ifndef QIUNIX_SHUTDOWN_H_
#include "io.h"
#include "const.h"

public
void shutdown() NO_RETURN;

// 通过ACPI来关机，这是真实物理机器的关机方式
public 
void ACPI_power_off() NO_RETURN;

public
void request_shutdown_port() NO_RETURN;

public
void reboot();

#endif // QIUNIX_SHUTDOWN_H_