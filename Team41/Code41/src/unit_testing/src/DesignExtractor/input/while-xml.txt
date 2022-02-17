<program>
	<procedure name=main>
		<stmts>
			<while>
				<eq>
					<var name=a></var>
					<const val=1></const>
				</eq>
				<stmts>
					<assign>
						<var name=x></var>
						<const val=1></const>
					</assign>
				</stmts>
			</while>
			<while>
				<ne>
					<const val=2></const>
					<var name=b></var>
				</ne>
				<stmts>
					<read>
						<var name=y></var>
					</read>
				</stmts>
			</while>
			<while>
				<lt>
					<var name=c></var>
					<var name=d></var>
				</lt>
				<stmts>
					<print>
						<var name=z></var>
					</print>
				</stmts>
			</while>
			<while>
				<gt>
					<var name=e></var>
					<const val=3></const>
				</gt>
				<stmts>
					<while>
						<ge>
							<const val=4></const>
							<var name=f></var>
						</ge>
						<stmts>
							<assign>
								<var name=w></var>
								<var name=v></var>
							</assign>
						</stmts>
					</while>
				</stmts>
			</while>
			<while>
				<and>
					<le>
						<var name=g></var>
						<const val=5></const>
					</le>
					<not>
						<or>
							<eq>
								<const val=6></const>
								<var name=h></var>
							</eq>
							<ne>
								<const val=7></const>
								<const val=8></const>
							</ne>
						</or>
					</not>
				</and>
				<stmts>
					<assign>
						<var name=t></var>
						<var name=u></var>
					</assign>
				</stmts>
			</while>
		</stmts>
	</procedure>
</program>