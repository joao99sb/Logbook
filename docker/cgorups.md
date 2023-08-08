## What do control groups( cgroups ) do?

1. Organize all processes in the system
2. Account for resource usage and gather utilization data
3. Limit or prioritzie resource utilization 

## Cgroups path

```bash
  ls /sys/fs/cgroups
```

# Get all process Id associated with some cgroup

1. This command will show you all processes that are in a cgroup inside this particular subsystem."
```bash
cat /sys/fs/cgroups/devices/task
```
In this case, I used devices' group just to exemplify



2. The /proc directory contains a directory that corresponds to each process ID.

To exemplify, Ill use the terminal PID to use.

To get, use this command:

in bash:
```bash
  echo $$
```

in fish
```bash
  echo $fish_pid
```

in my case, the result was: 170855

# Create a cgroup

1. you need to 
