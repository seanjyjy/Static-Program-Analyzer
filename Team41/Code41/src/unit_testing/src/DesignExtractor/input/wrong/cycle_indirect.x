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
                    <read>
                        <var name=x></var>
                    </read>
                </stmts>
            </if>
        </stmts>
    </procedure>
    <procedure name=proc2>
        <stmts>
            <while>
                <gt>
                    <var name=a></var>
                    <var name=b></var>
                </gt>
                <stmts>
                    <call>
                        <proc name=proc3></proc>
                    </call>
                </stmts>
            </while>
        </stmts>
    </procedure>
    <procedure name=proc3>
        <stmts>
            <if>
                <gt>
                    <var name=a></var>
                    <var name=b></var>
                </gt>
                <stmts>
                    <read>
                        <var name=x></var>
                    </read>
                </stmts>
                <stmts>
                    <call>
                        <proc name=proc1></proc>
                    </call>
                </stmts>
            </if>
        </stmts>
    </procedure>
</program>