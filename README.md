# Packet-Switched-Network-Simulator-Simulator
This program simulates a packet switching system, where packets arrive at different times and are processed by available switchers. It uses event-driven simulation to model packet arrivals, queueing, transmission, and completion.

### Features  
✅ Queue-based packet routing  
✅ Handles out-of-order packet delivery  
✅ Implements variable network delays 

###How it works
1️⃣ Packet Generation - Packets are created with random arrival times, transmission times, and priorities.
2️⃣ Queueing System - Packets enter a queue and wait for an available switcher.
3️⃣ Transmission Process - Available switchers pick packets based on priority and start transmission.
4️⃣ Completion & Statistics - Once all packets are processed, the program prints performance metrics.
