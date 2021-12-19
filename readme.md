# Multi-tasking Kernel for ATmega328P

This repo includes a simple multi-tasking kernel, allowing the registration and execution of multiple `task`s. In depth explanation of the code can be found on the following blog post: [Doing more than one thing at a time](https://github.com/francisrstokes/githublog/blob/main/2021/12/14/doing-more-than-one-thing.md).

## Still todo

- [ ] Task priority
- [ ] Yield until event
- [ ] Sleep / Delay
- [ ] System level abstraction
  - [ ] Accounting for pointer size differences across systems
  - [ ] Allow user to choose the timer backend, and automatically configure peripherals accordingly
