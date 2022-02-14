<program>
	<procedure name=n1iil>
		<stmts>
			<assign>
				<var name=singapore></var>
				<plus>
					<plus>
						<var name=china></var>
						<var name=malaysia></var>
					</plus>
					<var name=india></var>
				</plus>
			</assign>
			<read>
				<var name=korea></var>
			</read>
			<if>
				<or>
					<ge>
						<var name=korea></var>
						<var name=singapore></var>
					</ge>
					<gt>
						<var name=korea></var>
						<var name=japan></var>
					</gt>
				</or>
				<stmts>
					<print>
						<var name=daehanminguk></var>
					</print>
					<assign>
						<var name=singapore></var>
						<var name=china></var>
					</assign>
					<if>
						<gt>
							<var name=singapore></var>
							<var name=malaysia></var>
						</gt>
						<stmts>
							<print>
								<var name=majullah></var>
								<read>
									<var name=singlish></var>
								</read>
							</print>
						</stmts>
						<stmts>
							<assign>
								<var name=singapore></var>
								<var name=malaysia></var>
							</assign>
						</stmts>
					</if>
				</stmts>
			</if>
		</stmts>
	</procedure>
</program>
