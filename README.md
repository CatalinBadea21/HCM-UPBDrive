# Hybrid Control Module (HCM)

Embedded control logic for managing hybrid driving strategies in automotive systems. The module reads real-time vehicle data over CAN from the ECU and sends torque commands to the ESC based on user-selected strategies and system conditions.

## Features
- CAN communication with ECU and ESC
- 7 hybrid driving strategies (Freewheel, Manual, Launch Assist, Auto modes, Torque Fill)
- Input from rotary/binary selector and boost button
- Safety checks for gear and speed
- Real-time duty cycle computation and ESC command transmission

## Platform
- PSoC 5LP (Cypress)
- Developed with PSoC Creator

---

**Author**: Catalin Badea  
**Organization**: UPB Drive – Hybrid Department, 2025
