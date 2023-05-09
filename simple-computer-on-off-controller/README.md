# simple-computer-on-off-controller
Just a simple controller that turns on or reset a computer using an UART module and a relay

# Relay pin out
## Input
| **Relay Module** |  **UART MODULE** | 
|------------------|------------------|
| IN1              | RTS              | 
| IN2              | DTR              |

## Channel terminal block 1
| **Relay Module** |   **PC**     | 
|------------------|--------------|
| COM              | RESET        | 
| NO               | RESET_GROUND |

## Channel terminal block 2
| **Relay Module** |   **PC**     | 
|------------------|--------------|
| COM              | POWER        | 
| NO               | POWER_GROUND |
