<program>
	<procedure name=n3wim>
		<stmts>
			<assign>
				<var name=a></var>
				<times>
					<const val=10></const>
					<const val=20></const>
				</times>
			</assign>
			<while>
				<ge>
					<plus>
						<mod>
							<div>
								<times>
									<const val=2></const>
									<var name=a></var>
								</times>
								<var name=b></var>
							</div>
							<var name=c></var>
						</mod>
						<var name=d></var>
					</plus>
					<const val=1></const>
				</ge>
				<stmts>
					<assign>
						<var name=b></var>
						<times>
							<var name=a></var>
							<const val=10></const>
						</times>
					</assign>
					<if>
						<ne>
							<var name=a></var>
							<var name=b></var>
						</ne>
						<stmts>
							<assign>
								<var name=c></var>
								<plus>
									<var name=c></var>
									<var name=b></var>
								</plus>
							</assign>
							<while>
								<not>
									<not>
										<not>
											<not>
												<eq>
													<var name=a></var>
													<const val=0></const>
												</eq>
											</not>
										</not>
									</not>
								</not>
								<stmts>
									<print>
										<var name=x></var>
									</print>
									<if>
										<and>
											<eq>
												<var name=d></var>
												<const val=4></const>
											</eq>
											<and>
												<eq>
													<var name=c></var>
													<const val=3></const>
												</eq>
												<or>
													<eq>
														<var name=a></var>
														<const val=1></const>
													</eq>
													<eq>
														<var name=b></var>
														<const val=2></const>
													</eq>
												</or>
											</and>
										</and>
										<stmts>
											<print>
												<var name=x1></var>
											</print>
										</stmts>
										<stmts>
											<print>
												<var name=x2></var>
											</print>
										</stmts>
									</if>
									<print>
										<var name=x></var>
									</print>
								</stmts>
							</while>
							<print>
								<var name=x></var>
							</print>
						</stmts>
						<stmts>
							<read>
								<var name=x></var>
							</read>
							<while>
								<and>
									<not>
										<or>
											<eq>
												<var name=a></var>
												<var name=b></var>
											</eq>
											<eq>
												<var name=c></var>
												<var name=d></var>
											</eq>
										</or>
									</not>
									<eq>
										<var name=b></var>
										<var name=c></var>
									</eq>
								</and>
								<stmts>
									<read>
										<var name=x></var>
									</read>
									<if>
										<and>
											<eq>
												<const val=1></const>
												<var name=a></var>
											</eq>
											<eq>
												<var name=a></var>
												<const val=1></const>
											</eq>
										</and>
										<stmts>
											<print>
												<var name=x3></var>
											</print>
										</stmts>
										<stmts>
											<print>
												<var name=x4></var>
											</print>
										</stmts>
									</if>
									<read>
										<var name=x></var>
									</read>
								</stmts>
							</while>
							<read>
								<var name=x></var>
							</read>
						</stmts>
					</if>
					<assign>
						<var name=c></var>
						<times>
							<var name=b></var>
							<const val=10></const>
						</times>
					</assign>
				</stmts>
			</while>
			<assign>
				<var name=d></var>
				<times>
					<const val=20></const>
					<const val=30></const>
				</times>
			</assign>
		</stmts>
	</procedure>
</program>
