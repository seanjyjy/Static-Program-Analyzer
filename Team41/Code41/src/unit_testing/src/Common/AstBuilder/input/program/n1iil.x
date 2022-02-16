<program>
	<procedure name=n1iil>
		<stmts>
			<assign>
				<var name=sg></var>
				<plus>
					<plus>
						<var name=ch></var>
						<var name=my></var>
					</plus>
					<var name=ind></var>
				</plus>
			</assign>
			<read>
				<var name=kr></var>
			</read>
			<if>
				<or>
					<ge>
						<var name=kr></var>
						<var name=sg></var>
					</ge>
					<gt>
						<var name=kr></var>
						<var name=jap></var>
					</gt>
				</or>
				<stmts>
					<print>
						<var name=yay></var>
					</print>
					<assign>
						<var name=sg></var>
						<minus>
							<var name=ch></var>
							<var name=ind></var>
						</minus>
					</assign>
					<if>
						<ne>
							<var name=sg></var>
							<const val=999></const>
						</ne>
						<stmts>
							<print>
								<var name=help></var>
							</print>
						</stmts>
						<stmts>
							<read>
								<var name=ok></var>
							</read>
						</stmts>
					</if>
				</stmts>
				<stmts>
					<read>
						<var name=my></var>
					</read>
					<print>
						<var name=kr></var>
					</print>
					<if>
						<gt>
							<var name=sg></var>
							<var name=my></var>
						</gt>
						<stmts>
							<assign>
								<var name=ch></var>
								<times>
									<var name=kr></var>
									<var name=jap></var>
								</times>
							</assign>
						</stmts>
						<stmts>
							<assign>
								<var name=sg></var>
								<var name=my></var>
							</assign>
						</stmts>
					</if>
				</stmts>
			</if>
		</stmts>
	</procedure>
</program>
