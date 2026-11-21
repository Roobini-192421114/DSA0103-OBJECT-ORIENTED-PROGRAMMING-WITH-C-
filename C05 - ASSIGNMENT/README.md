# DSA01-<RegisterNumber>-Assignment5

**Name:** <Your Name>  **Register number:** <RegisterNumber>
**Course Outcome:** CO5 – Reliable software with runtime polymorphism and exceptions
**Case study:** Hospital billing and insurance claims

## Compiler
g++ 13.3.0 (Ubuntu 24.04), C++17. Every program is compiled with

    g++ -std=c++17 -Wall -Wextra <file>.cpp -o <name>

Tests are also run with `-fsanitize=address,undefined -g`; benchmarks with `-O2`.
Run everything: `./run_all.sh` (output saved in `results/run_all_output.txt`).

## Folder layout (one folder per task)
| Folder | Contents | How to run |
|---|---|---|
| Task_A2 | `A2_predict.cpp`, `A2_predict_nonvirtual_dtor.cpp`, `A2_alt_dynamic_cast.cpp` | `g++ -std=c++17 -Wall -Wextra A2_predict.cpp && ./a.out` |
| Task_A3 | `A3_alt_std_function.cpp` | as above |
| Task_A4 | `A4_alt_return_codes.cpp` | as above |
| Task_A5 | `A5_predict.cpp`, `A5_alt_manual_delete.cpp` | as above (the alt file is run with `-fsanitize=address`) |
| Task_B1 | `billing.hpp` (Bill, Payer and exception hierarchies), `B1_billing_tests.cpp`, `B1_complete.cpp`, `B1_error_nonvirtual_pure.cpp`, `B1_no_virtual_total.cpp`, `B1_destructor_experiment.cpp` (`-DVIRTUAL_DTOR`), `B1_alt_template.cpp` | see `run_all.sh` |
| Task_B2 | `B2_predict.cpp`, `B2_complete.cpp` (`-DRETHROW_COPY` for `throw e;`), `B2_throw_e_slicing.cpp`, `B2_errors.cpp` (`-DSKIP_BADALLOC` for ASan) | see `run_all.sh` |
| Task_B3 | `B3_correctness_10bills.cpp`, `B3_bench.cpp` (`-O2`), three raw benchmark outputs | `g++ -std=c++17 -O2 B3_bench.cpp && ./a.out` |
| Task_B4 | `B4_extension.cpp` (includes `../Task_B1/billing.hpp` unchanged), `B4_alt_plugin_rules.cpp` | see `run_all.sh` |
| diagrams | UML (Bill, Payer), vtable, exception hierarchy (`.dot` source + `.png`) | `dot -Tpng x.dot -o x.png` |

Each task folder also holds the captured program output (`*_output.txt`) and sanitizer reports (`*asan*.txt`).
Add your own screenshots of each run in a `screenshots/` sub-folder of every task.

## Results in one line per task
* **A2** – `500 ~OPBill ~Bill`; without a virtual `~Bill` only `~Bill` runs (undefined behaviour).
* **A3** – `std::function` payers give the same numbers as the abstract-class design.
* **A4** – return-code version prints identical results but every layer must forward the status.
* **A5** – `~f ~main caught 42`; manual `delete` in catch blocks leaks 80 bytes on the E2 path, RAII does not.
* **B1** – 12/12 bill×payer tests pass; `Complete the Code` prints `OP Ravi: 1700 / IP Meena: 47500 / Total 49200`; removing `virtual` from a pure `total()` is a compile error, from a concrete `total()` gives wrong totals (0); non-virtual destructor → ASan `new-delete-type-mismatch` and an 8000-byte leak.
* **B2** – `ok done caught negative [log too large] caught too large`; all engineered errors caught, `catch(...)` keeps program running, all destructors run.
* **B3** – 10-bill totals agree (155400); 10 M bills: switch ≈ 82 ms, variant ≈ 91 ms, virtual ≈ 117 ms; exceptions cost ≈ 0 at 0 %, ≈ 4× at 1 %, ≈ 80× at 50 % invalid.
* **B4** – new `TeleConsultBill` and `CorporatePayer` added without touching existing classes; 22/22 checks pass; ASan clean.

## Commit history
Commit after each task (see `git_commit_each_task.sh`). Do not upload everything on the last day.
