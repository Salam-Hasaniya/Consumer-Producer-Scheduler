# Consumer-Producer-Scheduler
Multi-threaded consumer-producer scheduling system using condition variables in C

## **Function Overview**

| Function               | Purpose                                              |
|------------------------|------------------------------------------------------|
| `produce_thread`       | Handles producer behavior—adds stock, waits if full |
| `consume_thread`       | Handles consumer behavior—removes stock, waits if empty |
| `pthread_mutex_lock`   | Ensures thread-safe access to stock management      |
| `pthread_cond_wait`    | Makes thread wait for condition (empty/full stock)  |
| `pthread_cond_signal`  | Wakes up waiting threads when stock changes         |
| `pthread_detach`       | Ensures resources are freed after thread completion |
| `main`                | Manages thread creation every TBD seconds            |



## **Synchronization Using `pthread`**

| Concept              | Explanation                                       |
|----------------------|---------------------------------------------------|
| `pthread_mutex_t`   | Ensures mutual exclusion on shared stock data     |
| `pthread_cond_t`    | Used for signaling producers & consumers          |
| `pthread_create`    | Spawns a new producer or consumer thread          |
| `pthread_detach`    | Allows threads to clean up after execution        |
| `pthread_cond_wait` | Blocks a thread until a condition is met          |
| `pthread_cond_signal` | Wakes up a waiting thread                        |



## **Consumer-Producer Logic Flow**

### **Producer Thread Flow**
1. **Generate random delivery size** (1-20 units).
2. **Check if stock is full:**
   - If **yes**, wait until there is space.
   - If **no**, deliver what fits & hold remaining.
3. **Signal consumers** waiting for stock.

### **Consumer Thread Flow**
1. **Generate random purchase request** (1-5 units).
2. **Check if stock is empty:**
   - If **yes**, wait for stock.
   - If **no**, consume what’s available & hold remaining request.
3. **Signal producers** if space becomes available.
