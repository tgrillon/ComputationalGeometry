async function updateContentOnceLoaded1()
{
    var data =`
==6547== Memcheck, a memory error detector<br />
==6547== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.<br />
==6547== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info<br />
==6547== Command: /home/runner/work/MeshToolBox/MeshToolBox/build/App/Test/AppUnitTests<br />
==6547== Parent PID: 6538<br />
==6547== <br />
==6547== <br />
==6547== <span class="valgrind_summary_title">HEAP SUMMARY:</span><br />
==6547== &nbsp; &nbsp; in use at exit: 0 bytes in 0 blocks<br />
==6547== &nbsp; total heap usage: 2,582 allocs, 2,582 frees, 494,791 bytes allocated<br />
==6547== <br />
==6547== All heap blocks were freed -- no leaks are possible<br />
==6547== <br />
==6547== For lists of detected and suppressed errors, rerun with: -s<br />
==6547== <span class="valgrind_summary_title">ERROR SUMMARY:</span> 0 errors from 0 contexts (suppressed: 0 from 0)<br />
`;
    var analysis_div = document.getElementById('valgrind.result1.Report');
    analysis_div.innerHTML=data;
}
updateContentOnceLoaded1();
