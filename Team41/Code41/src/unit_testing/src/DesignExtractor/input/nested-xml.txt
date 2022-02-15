<program>
	<procedure name=main>
		<stmts>
			<while>
				<ne>
					<var name=a></var>
					<const val=1></const>
				</ne>
				<stmts>
					<read>
						<var name=b></var>
					</read>
					<if>
						<gt>
							<var name=c></var>
							<const val=1></const>
						</gt>
						<stmts>
							<assign>
								<var name=d></var>
								<plus>
									<plus>
										<var name=e></var>
										<var name=f></var>
									</plus>
									<var name=g></var>
								</plus>
							</assign>
							<print>
								<var name=h></var>
							</print>
						</stmts>
						<stmts>
							<assign>
								<var name=i></var>
								<times>
									<const val=6></const>
									<const val=9></const>
								</times>
							</assign>
							<while>
								<or>
									<ne>
										<var name=j></var>
										<var name=k></var>
									</ne>
									<le>
										<var name=m></var>
										<const val=2></const>
									</le>
								</or>
								<stmts>
									<assign>
										<var name=n></var>
										<plus>
											<minus>
												<var name=p></var>
												<var name=q></var>
											</minus>
											<var name=r></var>
										</plus>
									</assign>
									<read>
										<var name=s></var>
									</read>
									<print>
										<var name=t></var>
									</print>
								</stmts>
							</while>
						</stmts>
					</if>
				</stmts>
			</while>
		</stmts>
	</procedure>
</program>