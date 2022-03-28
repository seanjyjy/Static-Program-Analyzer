<program>
    <procedure name=p1>
        <stmts>
            <call>
                <proc name=p2></proc>
            </call>
            <call>
                <proc name=p4></proc>
            </call>
            <assign>
                <var name=a></var>
                <plus>
                    <const val=1></const>
                    <var name=m></var>
                </plus>
            </assign>
        </stmts>
    </procedure>
    <procedure name=p2>
        <stmts>
            <call>
                <proc name=p3></proc>
            </call>
            <assign>
                <var name=b></var>
                <plus>
                    <const val=2></const>
                    <var name=n></var>
                </plus>
            </assign>
        </stmts>
    </procedure>
    <procedure name=p3>
        <stmts>
            <assign>
                <var name=c></var>
                <plus>
                    <const val=3></const>
                    <var name=o></var>
                </plus>
            </assign>
        </stmts>
    </procedure>
    <procedure name=p4>
        <stmts>
            <assign>
                <var name=d></var>
                <plus>
                    <const val=4></const>
                    <var name=p></var>
                </plus>
            </assign>
            <call>
                <proc name=p5></proc>
            </call>
            <call>
                <proc name=p6></proc>
            </call>
        </stmts>
    </procedure>
    <procedure name=p5>
        <stmts>
            <call>
                <proc name=p6></proc>
            </call>
            <assign>
                <var name=e></var>
                <plus>
                    <const val=5></const>
                    <var name=q></var>
                </plus>
            </assign>
        </stmts>
    </procedure>
    <procedure name=p6>
        <stmts>
            <assign>
                <var name=f></var>
                <plus>
                    <const val=6></const>
                    <var name=r></var>
                </plus>
            </assign>
        </stmts>
    </procedure>
</program>