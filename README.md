Format for storing numbers in file.
Integer numbers must be placed in file with ' ' (space character) separator after each number in the tape sequence.
Ex.
numbers.txt
```
"5555 1111 -1212 1234 -12312 23442 "
```

The settings file (named settings.json) for TapeDevice must be stored in the same directory as the external_sort binary.
Ex. settings json
```json
{
    "read_latency_ms":      100, 
    "write_latency_ms":     100,    
    "move_latency_ms":      100, 
    "mem_usage_percentages": 90 
}
```
