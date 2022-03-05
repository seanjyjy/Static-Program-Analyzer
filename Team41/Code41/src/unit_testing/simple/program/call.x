<program>
	<procedure name=call>
		<stmts>
			<call>
				<proc name=call></proc>
			</call>
			<if>
				<eq>
					<const val=0></const>
					<const val=0></const>
				</eq>
				<stmts>
					<call>
						<proc name=CALL></proc>
					</call>
					<assign>
						<var name=call></var>
						<plus>
							<const val=1></const>
							<const val=2></const>
						</plus>
					</assign>
				</stmts>
				<stmts>
					<call>
						<proc name=call123></proc>
					</call>
					<while>
						<eq>
							<var name=call></var>
							<const val=0></const>
						</eq>
						<stmts>
							<read>
								<var name=call></var>
							</read>
							<print>
								<var name=call></var>
							</print>
						</stmts>
					</while>
				</stmts>
			</if>
			<call>
				<proc name=call></proc>
			</call>
		</stmts>

	</procedure>
	<procedure name=CALL>
		<stmts>
			<call>
				<proc name=Call123></proc>
			</call>
			<assign>
				<var name=call></var>
				<plus>
					<plus>
						<const val=1></const>
						<const val=2></const>
					</plus>
					<const val=3></const>
				</plus>
			</assign>
			<read>
				<var name=Call123></var>
			</read>
			<print>
				<var name=Call123></var>
			</print>
		</stmts>
	</procedure>
	<procedure name=call123>
		<stmts>
			<if>
				<or>
					<eq>
						<var name=call></var>
						<var name=call></var>
					</eq>
					<ne>
						<var name=call></var>
						<var name=call></var>
					</ne>
				</or>
				<stmts>
					<call>
						<proc name=CALL></proc>
					</call>
				</stmts>
				<stmts>
					<print>
						<var name=x></var>
					</print>
				</stmts>
			</if>
		</stmts>
	</procedure>
	<procedure name=Call123>
		<stmts>
			<call>
				<proc name=call123></proc>
			</call>
			<call>
				<proc name=call123></proc>
			</call>
		</stmts>
	</procedure>
</program>