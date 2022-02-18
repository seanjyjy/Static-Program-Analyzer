<program>
    <procedure name=n1wwf>
        <stmts>
            <while>
                <and>
                    <eq>
                        <const val=1></const>
                        <const val=1></const>
                    </eq>
                    <eq>
                        <const val=2></const>
                        <const val=2></const>
                    </eq>
                </and>
                <stmts>
                    <while>
                        <or>
                            <eq>
                                <var name=a></var>
                                <var name=a></var>
                            </eq>
                            <eq>
                                <var name=b></var>
                                <var name=b></var>
                            </eq>
                        </or>
                        <stmts>
                            <print>
                                <var name=x></var>
                            </print>
                        </stmts>
                    </while>
                    <print>
                        <var name=x></var>
                    </print>
                </stmts>
            </while>
            <read>
                <var name=abook></var>
            </read>
            <print>
                <var name=thecops></var>
            </print>
        </stmts>
    </procedure>
</program>
