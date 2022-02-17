<program>
	<procedure name=n3wwf>
		<stmts>
			<while>
				<and>
					<lt>
						<const val=7></const>
						<plus>
							<const val=4></const>
							<const val=5></const>
						</plus>
					</lt>
					<or>
						<lt>
							<var name=a></var>
							<plus>
								<plus>
									<const val=2></const>
									<const val=3></const>
								</plus>
								<const val=4></const>
							</plus>
						</lt>
						<eq>
							<var name=b></var>
							<const val=99></const>
						</eq>
					</or>
				</and>
				<stmts>
					<while>
						<eq>
							<plus>
								<const val=1></const>
								<const val=4></const>
							</plus>
							<plus>
								<const val=2></const>
								<const val=3></const>
							</plus>
						</eq>
						<stmts>
							<if>
								<ne>
									<minus>
										<const val=9></const>
										<div>
											<var name=c></var>
											<var name=d></var>
										</div>
									</minus>
									<var name=e></var>
								</ne>
								<stmts>
									<while>
										<eq>
											<const val=1></const>
											<const val=1></const>
										</eq>
										<stmts>
											<print>
												<var name=x></var>
											</print>
										</stmts>
									</while>
									<read>
										<var name=x></var>
									</read>
								</stmts>
								<stmts>
									<print>
										<var name=g></var>
									</print>
								</stmts>
							</if>
							<assign>
							    <var name=h></var>
								<plus>
									<minus>
										<const val=9></const>
										<var name=h></var>
									</minus>
									<const val=2></const>
								</plus>
							</assign>
						</stmts>
					</while>
					<assign>
						<var name=i></var>
						<div>
							<var name=j></var>
							<div>
								<var name=j></var>
								<minus>
									<var name=k></var>
									<const val=88></const>
								</minus>
							</div>
						</div>
					</assign>
					<read>
						<var name=m></var>
					</read>
				</stmts>
			</while>
			<assign>
                <var name=p></var>
                <minus>
                    <var name=q></var>
                    <times>
                        <var name=r></var>
                        <var name=s></var>
                    </times>
                </minus>
            </assign>
		</stmts>
	</procedure>
</program>
