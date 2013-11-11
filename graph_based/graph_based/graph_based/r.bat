rem Various stochastic and error-driven runs
rem call run_stochastic_all.bat
rem call run_error_all.bat
rem call run_stochastic_b3_all.bat
rem call run_error_b3_all.bat
rem call run_greedy_all.bat
rem call run_greedy_b3_all.bat
rem call run_sort_all.bat
rem call run_sort_b3_all.bat
rem call run_hybrid_all.bat
rem call run_hybrid_b3_all.bat

rem Extra runs, included in run_greedy_all.bat and run_greedy_b3_al.bat, will be removed from here later
rem call run_greedy.bat 2 0
rem call run_greedy.bat 2 1
rem call run_greedy.bat 2 2
rem call run_greedy.bat 2 5
rem call run_greedy_b3.bat 2 0
rem call run_greedy_b3.bat 2 1
rem call run_greedy_b3.bat 2 2
rem call run_greedy_b3.bat 2 5

rem Top-1 and top-3 learning curve with 0 randomization
rem call run.bat 1 0
rem call run.bat 3 0

rem Top-1 and top-3 learning curve with 1 randomization (not being used, deleted)
rem call run.bat 1 1
rem call run.bat 3 1

rem Top-1 and top-3 for greedy2
rem Have to run it separately, otherwise, it would overwrite results from greedy1
call run_greedy2_summary.bat 1 0
call run_greedy2_summary.bat 3 0