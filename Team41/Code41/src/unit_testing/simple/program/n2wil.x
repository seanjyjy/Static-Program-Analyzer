<program>
	<procedure name=n2wil>
		<stmts>
			<print>
				<var name=a></var>
			</print>
			<while>
				<eq>
					<plus>
						<const val=1></const>
						<const val=1></const>
					</plus>
					<plus>
						<const val=2></const>
						<const val=2></const>
					</plus>
				</eq>
				<stmts>
					<print>
						<var name=y></var>
					</print>
					<if>
						<eq>
							<const val=1></const>
							<const val=1></const>
						</eq>
						<stmts>
							<print>
								<var name=z></var>
							</print>
							<while>
								<ne>
									<div>
										<const val=1></const>
										<const val=2></const>
									</div>
									<div>
										<const val=2></const>
										<const val=1></const>
									</div>
								</ne>
								<stmts>
									<print>
										<var name=x></var>
									</print>
								</stmts>
							</while>
						</stmts>
						<stmts>
							<print>
								<var name=a1></var>
							</print>
							<while>
								<eq>
									<const val=2></const>
									<const val=2></const>
								</eq>
								<stmts>
									<print>
										<var name=x></var>
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
