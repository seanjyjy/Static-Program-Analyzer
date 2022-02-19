<if>
    <and>
        <ge>
            <times>
                <times>
                    <var name=if></var>
                    <var name=else></var>
                </times>
                <var name=while></var>
            </times>
            <const val=1></const>
        </ge>
        <or>
            <and>
                <not>
                    <eq>
                        <var name=read></var>
                        <var name=print></var>
                    </eq>
                </not>
                <ne>
                    <var name=then></var>
                    <var name=procedure></var>
                </ne>
            </and>
            <eq>
                <var name=procedure></var>
                <const val=0></const>
            </eq>
        </or>
    </and>
    <stmts>
        <assign>
            <var name=read></var>
            <div>
                <times>
                    <var name=while></var>
                    <const val=10></const>
                </times>
                <var name=if></var>
            </div>
        </assign>
        <read>
            <var name=if></var>
        </read>
        <if>
            <or>
                <lt>
                    <var name=if></var>
                    <minus>
                        <var name=then></var>
                        <var name=while></var>
                    </minus>
                </lt>
                <eq>
                    <var name=read></var>
                    <var name=print></var>
                </eq>
            </or>
            <stmts>
                <assign>
                    <var name=procedure></var>
                    <plus>
                        <var name=if></var>
                        <var name=while></var>
                    </plus>
                </assign>
            </stmts>
            <stmts>
                <assign>
                    <var name=else></var>
                    <div>
                        <var name=then></var>
                        <var name=procedure></var>
                    </div>
                </assign>
                <if>
                    <eq>
                        <div>
                            <div>
                                <var name=while></var>
                                <var name=while></var>
                            </div>
                            <var name=while></var>
                        </div>
                        <times>
                            <times>
                                <var name=else></var>
                                <var name=else></var>
                            </times>
                            <var name=else></var>
                        </times>
                    </eq>
                    <stmts>
                        <print>
                            <var name=while></var>
                        </print>
                    </stmts>
                    <stmts>
                        <assign>
                            <var name=while></var>
                            <times>
                                <minus>
                                    <var name=if></var>
                                    <var name=else></var>
                                </minus>
                                <var name=procedure></var>
                            </times>
                        </assign>
                    </stmts>
                </if>
            </stmts>
        </if>
    </stmts>
    <stmts>
        <assign>
            <var name=print></var>
            <plus>
                <var name=if></var>
                <var name=else></var>
            </plus>
        </assign>
        <print>
            <var name=read></var>
        </print>
        <if>
            <eq>
                <var name=procedure></var>
                <times>
                    <times>
                        <const val=4></const>
                        <const val=4></const>
                    </times>
                    <const val=4></const>
                </times>
            </eq>
            <stmts>
                <assign>
                    <var name=else></var>
                    <times>
                        <var name=if></var>
                        <plus>
                            <var name=procedure></var>
                            <var name=else></var>
                        </plus>
                    </times>
                </assign>
                <if>
                    <lt>
                        <plus>
                            <plus>
                                <var name=program></var>
                                <var name=program></var>
                            </plus>
                            <var name=program></var>
                        </plus>
                        <plus>
                            <plus>
                                <var name=procedure></var>
                                <var name=procedure></var>
                            </plus>
                            <var name=procedure></var>
                        </plus>
                    </lt>
                    <stmts>
                        <print>
                            <var name=print></var>
                        </print>
                    </stmts>
                    <stmts>
                        <read>
                            <var name=read></var>
                        </read>
                    </stmts>
                </if>
            </stmts>
            <stmts>
                <assign>
                    <var name=read></var>
                    <div>
                        <minus>
                            <minus>
                                <var name=read></var>
                                <var name=if></var>
                            </minus>
                            <var name=read></var>
                        </minus>
                        <var name=else></var>
                    </div>
                </assign>
            </stmts>
        </if>
    </stmts>
</if>
