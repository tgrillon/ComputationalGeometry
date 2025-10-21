async function updateContentOnceLoaded1()
{
    var data =`
==6666== Memcheck, a memory error detector<br />
==6666== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.<br />
==6666== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info<br />
==6666== Command: /home/runner/work/MeshToolBox/MeshToolBox/build/App/Test/AppUnitTests<br />
==6666== Parent PID: 6657<br />
==6666== <br />
==6666== <br />
==6666== <span class="valgrind_summary_title">HEAP SUMMARY:</span><br />
==6666== &nbsp; &nbsp; in use at exit: 0 bytes in 0 blocks<br />
==6666== &nbsp; total heap usage: 2,514 allocs, 2,514 frees, 489,718 bytes allocated<br />
==6666== <br />
==6666== All heap blocks were freed -- no leaks are possible<br />
==6666== <br />
==6666== For lists of detected and suppressed errors, rerun with: -s<br />
==6666== <span class="valgrind_summary_title">ERROR SUMMARY:</span> 0 errors from 0 contexts (suppressed: 0 from 0)<br />
`;
    var analysis_div = document.getElementById('valgrind.result1.Report');
    analysis_div.innerHTML=data;
}
updateContentOnceLoaded1();
