async function updateContentOnceLoaded1()
{
    var data =`
==6245== Memcheck, a memory error detector<br />
==6245== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.<br />
==6245== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info<br />
==6245== Command: /home/runner/work/SandBox/SandBox/build/App/Test/AppUnitTests<br />
==6245== Parent PID: 6236<br />
==6245== <br />
==6245== <br />
==6245== <span class="valgrind_summary_title">HEAP SUMMARY:</span><br />
==6245== &nbsp; &nbsp; in use at exit: 0 bytes in 0 blocks<br />
==6245== &nbsp; total heap usage: 1,244 allocs, 1,244 frees, 265,648 bytes allocated<br />
==6245== <br />
==6245== All heap blocks were freed -- no leaks are possible<br />
==6245== <br />
==6245== For lists of detected and suppressed errors, rerun with: -s<br />
==6245== <span class="valgrind_summary_title">ERROR SUMMARY:</span> 0 errors from 0 contexts (suppressed: 0 from 0)<br />
`;
    var analysis_div = document.getElementById('valgrind.result1.Report');
    analysis_div.innerHTML=data;
}
updateContentOnceLoaded1();
