#!/usr/bin/env bash
# Compiles and runs every program. Usage: ./run_all.sh   (needs g++ >= 9, C++17)
set -u
F="-std=c++17 -Wall -Wextra"
S="-fsanitize=address,undefined -g"
mkdir -p build
run()  { echo; echo "=== $1 ==="; shift; "$@"; }
cc()   { g++ $F "$@"; }

run "A2 predict (virtual dtor)"        bash -c "cc() { g++ $F \"\$@\"; }; cc Task_A2/A2_predict.cpp -o build/a2 && build/a2"
run "A2 predict (non-virtual dtor)"    bash -c "g++ $F $S Task_A2/A2_predict_nonvirtual_dtor.cpp -o build/a2n 2>/dev/null; build/a2n"
run "A2 alt: dynamic_cast vs virtual"  bash -c "g++ $F -O2 Task_A2/A2_alt_dynamic_cast.cpp -o build/a2d && build/a2d"
run "A3 alt: std::function payers"     bash -c "g++ $F Task_A3/A3_alt_std_function.cpp -o build/a3 && build/a3"
run "A4 alt: return codes"             bash -c "g++ $F Task_A4/A4_alt_return_codes.cpp -o build/a4 && build/a4"
run "A5 predict"                       bash -c "g++ $F Task_A5/A5_predict.cpp -o build/a5 && build/a5"
run "A5 alt: manual delete (ASan shows leak)" bash -c "g++ $F $S Task_A5/A5_alt_manual_delete.cpp -o build/a5a && build/a5a; true"
run "B1 complete the code"             bash -c "g++ $F Task_B1/B1_complete.cpp -o build/b1c && build/b1c"
run "B1 non-virtual pure (compile error expected)" bash -c "g++ $F Task_B1/B1_error_nonvirtual_pure.cpp -o build/x 2>&1 | head -6; true"
run "B1.3 non-virtual total()"         bash -c "g++ $F Task_B1/B1_no_virtual_total.cpp -o build/b13 && build/b13"
run "B1.1 billing tests (ASan)"        bash -c "g++ $F $S Task_B1/B1_billing_tests.cpp -o build/b1t && build/b1t"
run "B1.4 virtual destructor OK"       bash -c "g++ $F $S -DVIRTUAL_DTOR Task_B1/B1_destructor_experiment.cpp -o build/b14ok && build/b14ok"
run "B1.4 NON-virtual destructor (ASan error expected)" bash -c "g++ $F $S Task_B1/B1_destructor_experiment.cpp -o build/b14bad 2>/dev/null; build/b14bad 2>&1 | head -8; true"
run "B1 alt: templates"                bash -c "g++ $F Task_B1/B1_alt_template.cpp -o build/b1a && build/b1a"
run "B2 predict"                       bash -c "g++ $F Task_B2/B2_predict.cpp -o build/b2p && build/b2p"
run "B2 complete the code"             bash -c "g++ $F Task_B2/B2_complete.cpp -o build/b2c && build/b2c"
run "B2 complete with 'throw e;'"      bash -c "g++ $F -DRETHROW_COPY Task_B2/B2_complete.cpp -o build/b2c2 && build/b2c2"
run "B2 throw vs throw e (slicing)"    bash -c "g++ $F Task_B2/B2_throw_e_slicing.cpp -o build/b2s && build/b2s"
run "B2 error conditions (incl. bad_alloc)" bash -c "g++ $F Task_B2/B2_errors.cpp -o build/b2e && build/b2e"
run "B2 error conditions under ASan (bad_alloc test skipped: ASan aborts on >1 TB)" bash -c "g++ $F $S -DSKIP_BADALLOC Task_B2/B2_errors.cpp -o build/b2ea && build/b2ea | tail -3"
run "B3.1 correctness on 10 bills"     bash -c "g++ $F Task_B3/B3_correctness_10bills.cpp -o build/b31 && build/b31"
run "B3.2/B3.3 benchmark (-O2)"        bash -c "g++ $F -O2 Task_B3/B3_bench.cpp -o build/b3 && build/b3"
run "B4.1 extension + earlier tests (ASan)" bash -c "g++ $F $S Task_B4/B4_extension.cpp -o build/b4 && build/b4 | tail -8"
run "B4 alt: rules from configuration" bash -c "g++ $F -O2 Task_B4/B4_alt_plugin_rules.cpp -o build/b4a && build/b4a"
