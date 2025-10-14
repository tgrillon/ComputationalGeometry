async function updateContentOnceLoaded1()
{
    var data =`
==6265== Memcheck, a memory error detector<br />
==6265== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.<br />
==6265== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info<br />
==6265== Command: /home/runner/work/SandBox/SandBox/build/App/Test/AppUnitTests<br />
==6265== Parent PID: 6256<br />
==6265== <br />
==6265== <br />
==6265== <span class="valgrind_summary_title">HEAP SUMMARY:</span><br />
==6265== &nbsp; &nbsp; in use at exit: 0 bytes in 0 blocks<br />
==6265== &nbsp; total heap usage: 1,530 allocs, 1,530 frees, 372,793 bytes allocated<br />
==6265== <br />
==6265== All heap blocks were freed -- no leaks are possible<br />
==6265== <br />
==6265== For lists of detected and suppressed errors, rerun with: -s<br />
==6265== <span class="valgrind_summary_title">ERROR SUMMARY:</span> 0 errors from 0 contexts (suppressed: 0 from 0)<br />
`;
    var analysis_div = document.getElementById('valgrind.result1.Report');
    analysis_div.innerHTML=data;
}
updateContentOnceLoaded1();
