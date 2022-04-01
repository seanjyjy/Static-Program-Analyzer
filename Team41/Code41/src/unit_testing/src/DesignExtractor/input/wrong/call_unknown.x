<program>
    <procedure name=proc1>
        <stmts>
            <if>
                <gt>
                    <var name=a></var>
                    <var name=b></var>
                </gt>
                <stmts>
                    <call>
                        <proc name=proc2></proc>
                    </call>
                </stmts>
                <stmts>
                    <call>
                        <proc name=proc3></proc>
                    </call>
                </stmts>
            </if>
        </stmts>
    </procedure>
    <procedure name=proc2>
        <stmts>
            <read>
               <var name=x></var>
            </read>
        </stmts>
    </procedure>
</program>