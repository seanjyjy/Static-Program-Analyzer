<program>
	<procedure name=n1wil>
		<stmts>
			<read>
				<var name=aespa></var>
			</read>
			<assign>
				<var name=blackpink></var>
				<plus>
					<var name=aespa></var>
					<const val=9999></const>
				</plus>
			</assign>
			<while>
				<gt>
					<var name=blackpink></var>
					<var name=aespa></var>
				</gt>
				<stmts>
					<assign>
						<var name=aespa></var>
						<plus>
							<var name=aespa></var>
							<const val=1></const>
						</plus>
					</assign>
					<print>
						<var name=blackpink></var>
					</print>
					<if>
						<ge>
							<var name=bts></var>
							<var name=blackpink></var>
						</ge>
						<stmts>
							<assign>
								<var name=blackpink></var>
								<plus>
									<var name=blackpink></var>
									<const val=1></const>
								</plus>
							</assign>
						</stmts>
						<stmts>
							<read>
								<var name=bts></var>
							</read>
						</stmts>
					</if>
				</stmts>
			</while>
		</stmts>
	</procedure>
</program>
